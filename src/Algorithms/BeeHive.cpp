#include <Algorithms/BeeHive.hpp>

#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <unordered_set>



namespace algorithm
{
    
BeeHive::BeeHive(
    const model::CNF& kCNF, 
    const model::Candidates& kCandidates
) : AlgorithmBase(kCNF), candidates_(kCandidates) {}

BeeHive::BeeHive(
    model::CNF&& kCNF, 
    model::Candidates&& kCandidates
) noexcept : AlgorithmBase(std::move(kCNF)), candidates_(std::move(kCandidates)) {}



const utils::BHExecutionResult BeeHive::Execute(
    const std::uint32_t kIterations,
    const std::uint32_t kPopulation,
    const std::uint32_t kScouts,
    const std::uint32_t kForagers,
    const std::uint32_t kOnlookers,
    utils::selection_function sf
)
{
    const auto kStartTime = std::chrono::high_resolution_clock::now();

    auto& candidates = this->candidates_.GetCandidates();

    std::vector<double> best_qualities;

    for (std::uint32_t i = 0; i < kIterations; i++)
    {
        for (auto& candidate : candidates)
        {
            candidate.EvaluateQualityFunction(this->cnf_);
            if (utils::DoubleEqual(candidate.GetQuality(), 1.0))
            {
                const auto kEndTime = std::chrono::high_resolution_clock::now();
                const auto kDuration = std::chrono::duration_cast<std::chrono::milliseconds>(kEndTime - kStartTime).count();

                best_qualities.emplace_back(1.0);

                return utils::BHExecutionResult{
                    i, 
                    best_qualities,
                    candidate.GetFunction(),
                    static_cast<std::uint32_t>(kDuration)
                };
            }
        }


        ScoutBeePhase(kScouts, candidates, sf);
        const auto kPropabilities = ForagerBeePhase(kForagers, candidates);
        OnlookerBeePhase(kOnlookers, kPropabilities, candidates);
        LocalSearchSolutions(candidates, sf);
        AbandonWorstSolutions(candidates);

        best_qualities.emplace_back(candidates.front().GetQuality());
        RestorePopulation(kPopulation, candidates);
        
    }

    const auto kEndTime = std::chrono::high_resolution_clock::now();
    const auto kDuration = std::chrono::duration_cast<std::chrono::milliseconds>(kEndTime - kStartTime).count();

    return utils::BHExecutionResult{
        kIterations, 
        best_qualities, 
        std::string{"Not enough iterations.\nPropably there is no solution."},
        static_cast<std::uint32_t>(kDuration)
    };
}



void BeeHive::ScoutBeePhase(
    const std::uint32_t kScouts,
    std::vector<model::Candidate>& candidates,
    utils::selection_function sf
) 
{
    switch (sf) 
    {
    case utils::selection_function::kRandom: 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(candidates.begin(), candidates.end(), gen);

        break;
    }
    
    case utils::selection_function::kLinear: 
    {
        std::vector<double> weights(candidates.size(), 1.0);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
        

        std::vector<model::Candidate> shuffled;
        shuffled.reserve(candidates.size());

        for (std::size_t i = 0; i < candidates.size(); ++i) 
        {
            std::size_t idx = dist(gen);
            shuffled.push_back(candidates[idx]);
        }

        candidates = std::move(shuffled);
        break;
    }

    case utils::selection_function::kExponential: 
    {
        std::vector<double> weights;
        for (std::size_t i = 0; i < candidates.size(); ++i) 
            weights.push_back(1.0 / (i + 1)); 
        
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
        
        std::vector<model::Candidate> shuffled;
        for (std::size_t i = 0; i < candidates.size(); ++i) 
        {
            std::size_t idx = dist(gen);
            shuffled.push_back(candidates[idx]);
        }

        candidates = std::move(shuffled);
        break;
    }

    default:
        break;
    }
    
    candidates.resize(kScouts);
}

std::vector<double> BeeHive::ForagerBeePhase(
    const std::uint32_t kForagers, 
    std::vector<model::Candidate>& candidates
)
{
    std::vector<double> probabilities;
    double totalQuality = 0.0;

    for (auto& candidate : candidates)
    {
        candidate.EvaluateQualityFunction(this->cnf_);
        totalQuality += candidate.GetQuality();
    }

    for (const auto& candidate : candidates)
        probabilities.push_back(candidate.GetQuality() / totalQuality);

    return probabilities;    
}

void BeeHive::OnlookerBeePhase( 
    const std::uint32_t kOnlookers,
    const std::vector<double>& kProbabilities,
    std::vector<model::Candidate>& candidates
)
{
    std::sort(candidates.begin(), candidates.end(), 
        [](const model::Candidate &lhs, const model::Candidate &rhs) { 
            return lhs.GetQuality() > rhs.GetQuality(); 
        }
    );

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::discrete_distribution<std::size_t> dist(kProbabilities.begin(), kProbabilities.end());


    std::unordered_set<std::size_t> selected_indices;
    while (selected_indices.size() < std::min(kOnlookers, static_cast<std::uint32_t>(candidates.size()))) 
        selected_indices.insert(dist(gen));
    

    std::vector<model::Candidate> selected;
    selected.reserve(selected_indices.size());
    for (size_t idx : selected_indices) 
        selected.push_back(std::move(candidates[idx]));

    candidates = std::move(selected);
}

void BeeHive::LocalSearchSolutions( 
    std::vector<model::Candidate>& candidates,
    utils::selection_function sf
)
{
    for (auto& candidate : candidates)
    {
        const std::string kCurrentCandidateString = candidate.GetFunction();
        const double kCurrentSolutionQuality = candidate.GetQuality();

        Mutate(candidate, sf);

        candidate.EvaluateQualityFunction(cnf_);
        const double kMutatedCandidateQuality = candidate.GetQuality();

        if (utils::DoubleLess(kMutatedCandidateQuality, kCurrentSolutionQuality))
            candidate = std::move(kCurrentCandidateString);        
    }
}

void BeeHive::AbandonWorstSolutions( 
    std::vector<model::Candidate>& candidates
)
{
    for (auto it = candidates.cbegin(); it != candidates.cend(); ++it)
    {
        if (utils::DoubleLess(it->GetQuality(), 0.75f))
            it = candidates.erase(it);
    }
}

void BeeHive::RestorePopulation(
    const std::uint32_t kTargetPopulation,
    std::vector<model::Candidate>& candidates
) 
{
    const size_t current_size = candidates.size();
    if (current_size >= kTargetPopulation) return;

    candidates.reserve(kTargetPopulation);
    while (candidates.size() < kTargetPopulation) 
        candidates.emplace_back(this->candidates_.GetCandidates().front().GetFunction().length());
}


void BeeHive::Mutate(
    model::Candidate& candidate,
    utils::selection_function mutation_strategy
) 
{
    std::string genetic_code = candidate.GetFunction();
    const size_t length = genetic_code.size();
    
    if (length == 0) return;
    

    std::random_device rd;
    std::mt19937 gen(rd());
    
    switch (mutation_strategy) 
    {
        case utils::selection_function::kRandom: 
        {
            std::uniform_int_distribution<size_t> dist(0, length - 1);
            size_t pos = dist(gen);
            genetic_code[pos] = (genetic_code[pos] == '1') ? '0' : '1';
            break;
        }
        
        case utils::selection_function::kLinear: 
        {
            std::vector<double> bit_weights(length, 1.0);
            
            std::discrete_distribution<size_t> dist(bit_weights.begin(), bit_weights.end());
            size_t pos = dist(gen);
            genetic_code[pos] = (genetic_code[pos] == '1') ? '0' : '1';
            break;
        }
        
        case utils::selection_function::kExponential: 
        {
            
            std::geometric_distribution<size_t> num_mutations(0.5); 
            size_t mutations = 1 + num_mutations(gen); 
            
            for (size_t i = 0; i < mutations && i < length; ++i) 
            {
                std::uniform_int_distribution<size_t> pos_dist(0, length - 1);
                size_t pos = pos_dist(gen);
                genetic_code[pos] = (genetic_code[pos] == '1') ? '0' : '1';
            }
            break;
        }
        
        default:
            throw std::invalid_argument("Unknown mutation strategy");
    }
}
} // namespace algorithm


