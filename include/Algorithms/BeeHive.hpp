#pragma once


#include <Core/AlgorithmBase.hpp>
#include <Core/Candidates.hpp>

#include <Enums/AlgorithmTypes.hpp>
#include <Utils/Utils.hpp>

#include <vector>

namespace algorithm
{

class BeeHive final 
    : public AlgorithmBase<
            utils::BHExecutionResult,
            std::uint32_t,
            std::uint32_t,
            std::uint32_t,
            std::uint32_t,
            std::uint32_t,
            utils::selection_function
        >
{
public:
    BeeHive( 
        const model::CNF& kCNF, 
        const model::Candidates& kCandidates 
    );

    BeeHive(
        model::CNF&& cnf,
        model::Candidates&& candidates
    ) noexcept;

    ~BeeHive() = default;

    const utils::BHExecutionResult Execute(
        const std::uint32_t kIterations,
        const std::uint32_t kPopulation,
        const std::uint32_t kScouts,
        const std::uint32_t kForagers,
        const std::uint32_t kOnlookers,
        utils::selection_function sf
    ) override;

private:

    void ScoutBeePhase( 
        const std::uint32_t kScouts,
        std::vector<model::Candidate>& candidates,
        utils::selection_function sf
    );

    std::vector<double> ForagerBeePhase( 
        const std::uint32_t kForagers,
        std::vector<model::Candidate>& candidates
    );

    void OnlookerBeePhase( 
        const std::uint32_t kOnlookers,
        const std::vector<double>& kProbabilities,
        std::vector<model::Candidate>& candidates
    );

    void LocalSearchSolutions( 
        std::vector<model::Candidate>& candidates,
        utils::selection_function sf
    ); 

    void AbandonWorstSolutions( std::vector<model::Candidate>& candidates ); 

    void RestorePopulation(
        const std::uint32_t kTargetPopulation,
        std::vector<model::Candidate>& candidates
    );


    void Mutate(
        model::Candidate& candidate,
        utils::selection_function mutation_strategy
    );

    
    std::unique_ptr<model::Candidates> candidates_;
};


} // namespace algorithm
