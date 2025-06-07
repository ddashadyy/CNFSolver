#pragma once

#include <Core/AlgorithmBase.hpp>
#include <Core/Candidates.hpp>
#include <Enums/AlgorithmTypes.hpp>
#include <Utils/Utils.hpp>
#include <vector>
#include <map>
#include <optional>

namespace algorithm 
{

class BeeHive final : public AlgorithmBase<
    utils::BHExecutionResult,
    std::uint32_t,
    std::uint32_t,
    std::uint32_t,
    std::uint32_t,
    double
> 
{
public:
    BeeHive( const model::CNF& cnf, const model::Candidates& candidates );
    BeeHive( model::CNF&& cnf, model::Candidates&& candidates ) noexcept;
    ~BeeHive() = default;

    const utils::BHExecutionResult Execute(
        const std::uint32_t iterations,
        const std::uint32_t scouts,
        const std::uint32_t honey_planters,
        const std::uint32_t surroundings,
        const double exclude_rate
    ) override;

private:
    std::vector<model::Candidate> CreateHoneyPlanters(
        const std::vector<model::Candidate>& candidates,
        const std::uint32_t honey_planters
    ) const;

    std::vector<model::Candidate> ExcludeWorst(
        std::vector<model::Candidate> candidates,
        const double exclude_rate
    ) const;

    std::map<model::Candidate, std::uint32_t> DistributeHoneyPlanters(
        const std::vector<model::Candidate>& best_candidates,
        const std::uint32_t honey_planters
    ) const;

    std::vector<model::Candidate> CreateNewHoneyPlanters(
        const std::map<model::Candidate, std::uint32_t>& distributed_bees,
        const std::uint32_t honey_planters,
        const std::uint32_t surroundings
    ) const;

    model::Candidate Mutate(
        const model::Candidate& candidate,
        const std::uint32_t surroundings
    ) const;

    std::vector<model::Candidate> RestorePopulation(
        std::vector<model::Candidate> candidates,
        const std::uint32_t target_population
    ) const;

    model::Candidate best_candidate_;
    std::unique_ptr<model::Candidates> candidates_;
};

} // namespace algorithm