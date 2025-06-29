#include <Algorithms/SimulatedAnnealing.hpp>

#include <random>
#include <chrono>
#include <algorithm>


namespace algorithm
{

algorithm::SimulatedAnnealing::SimulatedAnnealing(
    const model::CNF& kCNF, const model::Candidate& kCandidate
) : AlgorithmBase(kCNF), candidate_(std::make_unique<model::Candidate>(kCandidate)) {}

algorithm::SimulatedAnnealing::SimulatedAnnealing(
    model::CNF&& kCNF, model::Candidate&& kCandidate
) noexcept : AlgorithmBase(std::move(kCNF)), candidate_(std::make_unique<model::Candidate>(std::move(kCandidate))) {}


const utils::SAExecutionResult algorithm::SimulatedAnnealing::Execute(
    const std::uint32_t kIterations, 
    const double kInitialTemperature, 
    const double kMinimalTemperature, 
    const double kCoolingRate, 
    utils::cooling_type ct
)
{
    const auto kStartTime = std::chrono::high_resolution_clock::now();

    double currentTemperature = kInitialTemperature;

    model::Candidate bestSolution{*candidate_};
    double bestEnergy = EvaluateEnergy(*candidate_);

    std::vector<double> energies{};
    std::vector<double> temperatures{};
    std::vector<double> best_progressive_qualities;
    double current_best_quality = 0.0;

    std::string algorithmResult{"The closest solution "};

    for (std::uint32_t i = 0; i < kIterations; i++)
    {
        auto currentEnergy = bestEnergy;

        if (utils::DoubleEqual(currentEnergy, 0))
        {
            const auto kEndTime = std::chrono::high_resolution_clock::now();
            const auto kDuration = std::chrono::duration_cast<std::chrono::milliseconds>(kEndTime - kStartTime).count();

            energies.emplace_back(currentEnergy);
            temperatures.emplace_back(currentTemperature);

            if (utils::DoubleGreater(1.0, current_best_quality)) 
                best_progressive_qualities.emplace_back(1.0);

            return utils::SAExecutionResult{
                i,
                energies,
                temperatures,
                best_progressive_qualities,
                candidate_->GetFunction(),
                static_cast<std::uint32_t>(kDuration)
            };
        }

        auto newCandidate = GetCandidateNeighbor(kCoolingRate);
        double newEnergy = EvaluateEnergy(newCandidate);

        if (AcceptSolution(currentEnergy, newEnergy, currentTemperature))
        {
            candidate_ = std::make_unique<model::Candidate>(std::move(newCandidate));
            currentEnergy = newEnergy;
        }

        if (utils::DoubleLess(currentEnergy, bestEnergy))
        {
            bestSolution = *candidate_;
            bestEnergy = currentEnergy;            
        }

        
        if (utils::DoubleLessOrEqual(currentTemperature, kMinimalTemperature)) 
        {
            algorithmResult = "Init temp reaches minimal.\n The closest solution: ";
            break;
        }
        
        energies.emplace_back(bestEnergy);
        temperatures.emplace_back(currentTemperature);

        currentTemperature = Cool(currentTemperature, kCoolingRate, ct);

        double iteration_best = candidate_->GetQuality();
        
        if (utils::DoubleGreater(iteration_best, current_best_quality)) 
        {
            best_progressive_qualities.emplace_back(iteration_best);
            current_best_quality = iteration_best;
        }

    }

    const auto kEndTime = std::chrono::high_resolution_clock::now();
    const auto kDuration = std::chrono::duration_cast<std::chrono::milliseconds>(kEndTime - kStartTime).count();

    return utils::SAExecutionResult{
        kIterations,
        energies,
        temperatures,
        best_progressive_qualities,
        algorithmResult + bestSolution.GetFunction(),
        static_cast<std::uint32_t>(kDuration)
    };
}


double algorithm::SimulatedAnnealing::EvaluateEnergy(model::Candidate& candidate)
{
    candidate.EvaluateQualityFunction(*cnf_);
    return 1.0 - candidate.GetQuality();
}

model::Candidate SimulatedAnnealing::GetCandidateNeighbor(const double kCoolingRate)
{
    model::Candidate candidate{*candidate_};

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint32_t> dist(0, candidate.GetFunction().size() - 1);

    std::uint32_t num_flips = 1 + static_cast<std::uint32_t>(kCoolingRate * (candidate.GetFunction().size() / 2));
    num_flips = std::min(num_flips, static_cast<std::uint32_t>(candidate.GetFunction().size()));

    for (std::uint32_t i = 0; i < num_flips; ++i) 
    {
        std::uint32_t idx = dist(gen);
        auto& bit = candidate.GetFunction()[idx];
        bit = (bit == '0') ? '1' : '0';
    }

    return candidate;
}

bool SimulatedAnnealing::AcceptSolution(const double kCurrentEnergy, const double kNewEnergy, const double kTemperature) const
{
    const double kDelta = kNewEnergy - kCurrentEnergy;
    if (utils::DoubleLessOrEqual(kDelta, 0.0)) return true;
    

    static std::random_device rd;  
    static std::mt19937 rng(rd());

    const double kPropability = std::exp(-kDelta / kTemperature);

    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
    return utils::DoubleLess(prob_dist(rng), kPropability);
}

double SimulatedAnnealing::Cool(
    const double kCurrentTemperature,
    const double kCoolingRate,
    utils::cooling_type ct
) const
{
    switch (ct)
    {
    case utils::cooling_type::kLinear:
        return kCurrentTemperature - kCoolingRate;
    
    case utils::cooling_type::kExponential:
        return kCurrentTemperature * kCoolingRate;
    
    default:
        return 0.0;
    }
}


} // namespace algorithm


