#include <Benchmarks/BenchmarkSA.hpp>
#include <Algorithms/SimulatedAnnealing.hpp>

#include <fstream>
#include <algorithm>

namespace benchmark
{

utils::SAExecutionResult BenchmarkSA::Run()
{
    std::vector<utils::SAExecutionResult> results;

    std::ofstream fileBench("SA_benchmark.txt");
    if (fileBench.is_open())
    {
        for (std::uint32_t i = 0; i < 10; i++)
        {
            const auto cnf = CreateTestCNF();
            const auto candidate = CreateTestCandidate();

            auto algorithm = std::make_unique<algorithm::SimulatedAnnealing>(*cnf, *candidate);

            const auto result = algorithm->Execute(
                1000,
                1000.0f,
                0.01f,
                0.95,
                utils::cooling_type::kExponential
            );


            fileBench << "Iteration benchmark number: " << i << '\n';
            fileBench << "Duration of execution: " << result.duration << '\n';
            fileBench << "Execution iteration: " << result.iterations << '\n';
            fileBench << result.solution << '\n' << '\n';

            if (result.iterations > 1 && result.iterations < 1000 && result.duration > 0) 
                results.emplace_back(result);
        }
    }

    fileBench.close();
    
    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) { 
            return a.duration > b.duration; 
        }
    );

    return results.front();
}


} // namespace benchmark
