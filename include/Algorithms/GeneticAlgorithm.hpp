#pragma once

#include <Utils/Utils.hpp>
#include <Core/AlgorithmBase.hpp>
#include <Core/CNF.hpp>
#include <Core/Candidates.hpp>
#include <Enums/AlgorithmTypes.hpp>

namespace algorithm
{

class GeneticAlgorithm final 
    : public AlgorithmBase<
            utils::GAExecutionResult, 
            std::size_t,
            std::size_t,
            std::size_t,
            std::size_t,
            std::size_t,
            utils::selection_function   
        >
{
public:

    GeneticAlgorithm( 
        const model::CNF& kCNF,
        const model::Candidates& kCandidates
    );
    GeneticAlgorithm( 
        model::CNF&& cnf,
        model::Candidates&& candidates
    ) noexcept;

    ~GeneticAlgorithm() = default;

    utils::GAExecutionResult Execute(
        const std::size_t kIterations,
        const std::size_t kPopulation,
        const std::size_t kCrossovers,
        const std::size_t kMutations,
        const std::size_t kAmountGensMutation,
        utils::selection_function sf
    ) override;

private:

    void Crossover(
        std::vector<model::Candidate>& candidates,
        const std::size_t kCrossovers,
        const std::size_t kPopulation,
        utils::selection_function sf
    );

    void Mutate(
        std::vector<model::Candidate>& candidates,
        const std::size_t kPopulation,
        const std::size_t kMutations,
        const std::size_t kAmountGensMutation,
        utils::selection_function sf
    );

    void DoSelection(
        std::vector<model::Candidate>& candidates,
        const std::size_t kPopulation
    );

    model::Candidates candidates_;
};

} // namespace algorithm

