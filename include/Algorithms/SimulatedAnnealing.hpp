#pragma once

#include <Core/AlgorithmBase.hpp>
#include <Core/Candidate.hpp>
#include <Core/CNF.hpp>
#include <Utils/Utils.hpp>
#include <Enums/AlgorithmTypes.hpp>

namespace algorithm
{

class SimulatedAnnealing final 
    : public AlgorithmBase<
            utils::SAExecutionResult,
            std::uint32_t,
            double,
            double,
            double,
            utils::cooling_type
        >
{
public:

    SimulatedAnnealing() = default;

    SimulatedAnnealing(
        const model::CNF& kCNF,
        const model::Candidate& kCandidate
    );
    SimulatedAnnealing(
        model::CNF&& cnf,
        model::Candidate&& candidate
    ) noexcept;

    ~SimulatedAnnealing() = default;

    const utils::SAExecutionResult Execute(
        const std::uint32_t kIterations,
        const double kInitialTemperature,
        const double kMinimalTemperature,
        const double kCoolingRate,
        utils::cooling_type ct
    ) override;


private:
    double EvaluateEnergy( model::Candidate& candidate );

    model::Candidate GetCandidateNeighbor( const double kCoolingRate );

    bool AcceptSolution(
        const double kCurrentEnergy,
        const double kNewEnergy,
        const double kTemperature
    ) const;

    double Cool(
        const double kCurrentTemperature,
        const double kCoolingRate,
        utils::cooling_type ct
    ) const;

    
    std::unique_ptr<model::Candidate> candidate_;
};


} // namespace algorithm
