#include <Benchmarks/BenchmarkBH.hpp>
#include <Algorithms/BeeHive.hpp>


#include <fstream>
#include <algorithm>

namespace benchmark
{

utils::BHExecutionResult BenchmarkBH::Run()
{
    std::vector<utils::BHExecutionResult> results;

    std::ofstream fileBench("BH_benchmark.txt");
    if (fileBench.is_open())
    {
        for (std::uint32_t i = 0; i < 10; i++)
        {
            const auto cnf = CreateTestCNF();
            const auto candidates = CreateTestCandidates(10);

            auto algorithm = std::make_unique<algorithm::BeeHive>(*cnf, *candidates);

            const auto result = algorithm->Execute(
                1000,
                5,
                5,
                3,
                0.65f 
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
