#include <Algorithms/BeeHive.hpp>

#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>


namespace algorithm 
{

BeeHive::BeeHive(const model::CNF& cnf, const model::Candidates& candidates)
    : AlgorithmBase(cnf), candidates_(std::make_unique<model::Candidates>(candidates)) {}

BeeHive::BeeHive(model::CNF&& cnf, model::Candidates&& candidates) noexcept
    : AlgorithmBase(std::move(cnf)), candidates_(std::make_unique<model::Candidates>(std::move(candidates))) {}

const utils::BHExecutionResult BeeHive::Execute(
    const std::uint32_t iterations,
    const std::uint32_t scouts,
    const std::uint32_t honey_planters,
    const std::uint32_t surroundings,
    const double exclude_rate
) 
{
    if (!candidates_ || candidates_->GetCandidates().empty()) 
        return {0, {}, {}, "No candidates provided", 0};

    auto candidates = candidates_->GetCandidates();
    best_candidate_ = candidates.front();

    const std::uint32_t population = scouts + honey_planters;
    std::vector<double> best_qualities;
    std::vector<double> progressive_qualities;
    double best_progressive_quality = 0.0; 

    const auto start_time = std::chrono::high_resolution_clock::now();

    candidates = CreateHoneyPlanters(candidates, honey_planters);

    for (std::uint32_t i = 0; i < iterations; ++i) 
    {
        for (auto& candidate : candidates) 
        {
            candidate.EvaluateQualityFunction(*cnf_);
            if (utils::DoubleEqual(candidate.GetQuality(), 1.0)) 
            {
                best_candidate_ = candidate;
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start_time
                ).count();

                best_qualities.push_back(1.0);
                progressive_qualities.push_back(1.0);

                return {
                    i, 
                    best_qualities,
                    progressive_qualities,
                    best_candidate_.GetFunction(), 
                    static_cast<std::uint32_t>(duration)
                };
            }
        }

        candidates = ExcludeWorst(std::move(candidates), exclude_rate);
        auto distribution = DistributeHoneyPlanters(candidates, honey_planters);
        auto new_planters = CreateNewHoneyPlanters(distribution, honey_planters, surroundings);
        candidates = RestorePopulation(std::move(new_planters), population);

        auto best_iter = std::max_element(candidates.begin(), candidates.end(),
            [](const auto& a, const auto& b) { return a.GetQuality() < b.GetQuality(); }
        );
        
        if (best_iter != candidates.end()) 
        {
            best_candidate_ = *best_iter;
            double current_quality = best_candidate_.GetQuality();
            
            best_qualities.push_back(current_quality);
            
            if (current_quality > best_progressive_quality) 
            {
                best_progressive_quality = current_quality;
                progressive_qualities.push_back(best_progressive_quality);
            } 
            else progressive_qualities.push_back(best_progressive_quality);
        }
        else
        {
            best_qualities.push_back(0.0);
            progressive_qualities.push_back(best_progressive_quality);
        }
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start_time).count();

    return {
        iterations,
        best_qualities,
        progressive_qualities,
        "Not enough iterations. Probably there is no solution.",
        static_cast<std::uint32_t>(duration)
    };
}

std::vector<model::Candidate> BeeHive::CreateHoneyPlanters(
    const std::vector<model::Candidate>& candidates,
    const std::uint32_t honey_planters
) const 
{
    if (candidates.empty()) return {};

    auto sorted = candidates;
    for (auto& c : sorted) c.EvaluateQualityFunction(*cnf_);

    std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
        return a.GetQuality() > b.GetQuality();
    });

    if (sorted.size() > honey_planters) sorted.resize(honey_planters);
    
    return sorted;
}

std::vector<model::Candidate> BeeHive::ExcludeWorst(
    std::vector<model::Candidate> candidates,
    const double exclude_rate
) const 
{
    if (candidates.empty()) return {};

    auto new_end = std::remove_if(candidates.begin(), candidates.end(),
        [exclude_rate](const auto& c) {
            return utils::DoubleLess(c.GetQuality(), exclude_rate);
        }
    );

    if (new_end == candidates.begin()) 
    {
        auto best = std::max_element(candidates.begin(), candidates.end(),
            [](const auto& a, const auto& b) { return a.GetQuality() < b.GetQuality(); });

        if (best != candidates.end()) return { *best };
        
        return {};
    }

    candidates.erase(new_end, candidates.end());

    return candidates;
}

std::map<model::Candidate, std::uint32_t> BeeHive::DistributeHoneyPlanters(
    const std::vector<model::Candidate>& best_candidates,
    const std::uint32_t honey_planters
) const 
{
    std::map<model::Candidate, std::uint32_t> distribution;
    if (best_candidates.empty() || honey_planters == 0) return distribution;

    double total_quality = std::accumulate(best_candidates.begin(), best_candidates.end(), 0.0,
        [](double sum, const auto& c) { return sum + c.GetQuality(); });

    std::vector<std::uint32_t> counts(best_candidates.size(), 0);

    if (total_quality > 0) 
        for (size_t i = 0; i < best_candidates.size(); ++i) 
            counts[i] = static_cast<std::uint32_t>(std::round(
                best_candidates[i].GetQuality() / total_quality * honey_planters)
            );
        
    else std::fill(counts.begin(), counts.end(), honey_planters / counts.size());
    
    for (size_t i = 0; i < best_candidates.size(); ++i) 
        if (counts[i] > 0) 
            distribution[best_candidates[i]] = counts[i];

    return distribution;
}

std::vector<model::Candidate> BeeHive::CreateNewHoneyPlanters(
    const std::map<model::Candidate, std::uint32_t>& distributed_bees,
    const std::uint32_t honey_planters,
    const std::uint32_t surroundings
) const 
{
    std::vector<model::Candidate> new_planters;
    new_planters.reserve(honey_planters);

    for (const auto& [candidate, count] : distributed_bees) 
        for (std::uint32_t i = 0; i < count; ++i) 
            new_planters.push_back(Mutate(candidate, surroundings));

    if (new_planters.size() < honey_planters) 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, new_planters.empty() ? 0 : new_planters.size() - 1);

        while (new_planters.size() < honey_planters) 
        {
            auto new_candidate = new_planters.empty()
                ? model::Candidate(std::string(candidates_->GetCandidates()[0].GetFunction().size(), '0'))
                : Mutate(new_planters[dist(gen)], surroundings);

            new_planters.push_back(std::move(new_candidate));
        }
    }

    return new_planters;
}

model::Candidate BeeHive::Mutate(
    const model::Candidate& candidate,
    const std::uint32_t surroundings
) const 
{
    std::string func = candidate.GetFunction();
    if (func.empty() || surroundings == 0) return candidate;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, func.size() - 1);

    for (std::uint32_t i = 0; i < surroundings; ++i) 
    {
        size_t pos = dist(gen);
        func[pos] = (func[pos] == '0') ? '1' : '0';
    }

    model::Candidate mutated(std::move(func));
    mutated.EvaluateQualityFunction(*cnf_);
    return mutated;
}

std::vector<model::Candidate> BeeHive::RestorePopulation(
    std::vector<model::Candidate> candidates,
    const std::uint32_t target_population
) const 
{
    if (candidates.size() >= target_population) return candidates;
    if (!candidates_ || candidates_->GetCandidates().empty()) return candidates;

    const size_t func_size = candidates_->GetCandidates()[0].GetFunction().size();
    while (candidates.size() < target_population) {
        candidates.emplace_back(func_size);
        candidates.back().EvaluateQualityFunction(*cnf_);
    }

    return candidates;
}

} // namespace algorithm