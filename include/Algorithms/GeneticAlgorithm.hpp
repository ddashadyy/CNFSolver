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
            std::uint32_t,
            std::uint32_t,
            std::uint32_t,
            std::uint32_t,
            std::uint32_t,
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

    const utils::GAExecutionResult Execute(
        const std::uint32_t kIterations,
        const std::uint32_t kPopulation,
        const std::uint32_t kCrossovers,
        const std::uint32_t kMutations,
        const std::uint32_t kAmountGensMutation,
        utils::selection_function sf
    ) override;

private:

    void Crossover(
        std::vector<model::Candidate>& candidates,
        const std::uint32_t kCrossovers,
        const std::uint32_t kPopulation,
        utils::selection_function sf
    );

    void Mutate(
        std::vector<model::Candidate>& candidates,
        const std::uint32_t kPopulation,
        const std::uint32_t kMutations,
        const std::uint32_t kAmountGensMutation,
        utils::selection_function sf
    );

    void DoSelection(
        std::vector<model::Candidate>& candidates,
        const std::uint32_t kPopulation
    );

    model::Candidates candidates_;
};

} // namespace algorithm

