#include <Algorithms/GeneticAlgorithm.hpp>

#include <cmath>
#include <chrono>
#include <random>
#include <algorithm>
#include <iostream>

namespace algorithm
{

GeneticAlgorithm::GeneticAlgorithm(
    const model::CNF& kCNF,
    const model::Candidates& kCandidates
) : AlgorithmBase(kCNF), candidates_(kCandidates) {}

GeneticAlgorithm::GeneticAlgorithm(
    model::CNF&& cnf, 
    model::Candidates&& candidates
) noexcept : AlgorithmBase(std::move(cnf)), candidates_(candidates) {}

utils::GAExecutionResult GeneticAlgorithm::Execute(
    const std::size_t kIterations, 
    const std::size_t kPopulation, 
    const std::size_t kCrossovers, 
    const std::size_t kMutations, 
    const std::size_t kAmountGensMutation, 
    utils::selection_function sf
)
{
    const auto kStartTime = std::chrono::high_resolution_clock::now();

    auto& candidates = this->candidates_.GetCandidates();
    std::size_t iteration_counter = 0;

    std::vector<double> best_qualities;

    for (std::size_t i = 0; i < kIterations; i++)
    {
        for (auto& candidate : candidates)
        {
            candidate.EvaluateQualityFunction(this->cnf_);
            if (std::fabs(candidate.GetQuality() - 1.0) < utils::EPSILON)
            {

                const auto kEndTime = std::chrono::high_resolution_clock::now();
                const auto kDuration = std::chrono::duration_cast<std::chrono::milliseconds>(kEndTime - kStartTime).count();

                if (iteration_counter == 0) 
                    return utils::GAExecutionResult{
                        iteration_counter,
                        std::vector<double>{candidate.GetQuality()},
                        candidate.GetFunction(),
                        static_cast<std::size_t>(kDuration)
                    };
                
                best_qualities.emplace_back(1.0);

                return utils::GAExecutionResult{
                        iteration_counter,
                        best_qualities,
                        candidate.GetFunction(),
                        static_cast<std::size_t>(kDuration)
                    };
            }
        }

        Crossover(candidates, kCrossovers, kPopulation, sf);
        Mutate(candidates, kPopulation, kMutations, kAmountGensMutation, sf);
        DoSelection(candidates, kPopulation);

        best_qualities.emplace_back(candidates.front().GetQuality());
        ++iteration_counter;
    }

    const auto kEndTime = std::chrono::high_resolution_clock::now();
    const auto kDuration = std::chrono::duration_cast<std::chrono::milliseconds>(kEndTime - kStartTime).count();

    return utils::GAExecutionResult{
        iteration_counter, 
        best_qualities, 
        std::string{"there is no solution"},
        static_cast<std::size_t>(kDuration)
    };
}

void GeneticAlgorithm::Crossover(
    std::vector<model::Candidate>& candidates, 
    const std::size_t kCrossovers, 
    const std::size_t kPopulation,
    utils::selection_function sf
)
{
    if (candidates.empty() || candidates[0].GetFunction().empty() || candidates.size() < 2) 
        return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::size_t func_size = candidates[0].GetFunction().size();
    std::uniform_int_distribution<std::size_t> distrib(0, candidates.size() - 1);

    for (std::size_t i = 0; i < kCrossovers && candidates.size() < kPopulation; i++)
    {
        std::size_t first_parent_index = 0;
        std::size_t second_parent_index = 0;

        do 
        {
            switch (sf)
            {
            case utils::selection_function::kRandom:
                first_parent_index = distrib(gen);
                do 
                {
                    second_parent_index = distrib(gen);
                } while (second_parent_index == first_parent_index);
                break;

            case utils::selection_function::kLinear:
                first_parent_index = i % candidates.size();
                second_parent_index = (i + 1) % candidates.size();

                if (second_parent_index == first_parent_index) 
                    second_parent_index = (first_parent_index + 1) % candidates.size();
                
                break;

            case utils::selection_function::kExponential:
                first_parent_index = (i * i) % candidates.size();
                second_parent_index = ((i + 1) * (i + 1)) % candidates.size();

                if (second_parent_index == first_parent_index) 
                    second_parent_index = (first_parent_index + 1) % candidates.size();
                
                break;

            default:
                first_parent_index = 0;
                second_parent_index = 1 % candidates.size();
                break;
            }
        } while (first_parent_index == second_parent_index);

        std::uniform_int_distribution<std::size_t> distrib_point(1, func_size - 1);
        std::size_t point = distrib_point(gen);


        std::string child =
            candidates[first_parent_index].GetFunction().substr(0, point) +
            candidates[second_parent_index].GetFunction().substr(point);

        candidates.emplace_back(child);
    }
}

void GeneticAlgorithm::Mutate(
    std::vector<model::Candidate>& candidates, 
    const std::size_t kPopulation, 
    const std::size_t kMutations, 
    const std::size_t kAmountGensMutation,
    utils::selection_function sf
)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distrib(0, kPopulation - 1);

    for (std::size_t i = 0; i < kMutations; i++)
    {
        std::size_t candidate_index_to_mutate = 0;

        switch (sf)
        {
        case utils::selection_function::kRandom:
            candidate_index_to_mutate = distrib(gen);
            break;

        case utils::selection_function::kLinear:
            candidate_index_to_mutate = i % (candidates.size());
            break;

        case utils::selection_function::kExponential:
            candidate_index_to_mutate = (i * i) % (candidates.size());
            break;

        default:
            break;
        }

        for (std::size_t j = 0; j < kAmountGensMutation; j++)
        {
            std::uniform_int_distribution<std::size_t> distrib(0, candidates[i].GetFunction().size() - 1);
            std::size_t mutation_point = distrib(gen);

            candidates[candidate_index_to_mutate].GetFunction()[mutation_point] =
                (candidates[candidate_index_to_mutate].GetFunction()[mutation_point] == '0') ? '1' : '0';
        }
    }
}

void GeneticAlgorithm::DoSelection(
    std::vector<model::Candidate>& candidates, 
    const std::size_t kPopulation
)
{
    std::sort(candidates.begin(), candidates.end(), [](const model::Candidate &lhs, const model::Candidate &rhs)
              { return lhs.GetQuality() < rhs.GetQuality(); });

    candidates.resize(kPopulation);
}

} // namespace algorithm
