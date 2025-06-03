#include <Benchmarks/BenchmarkSA.hpp>
#include <Algorithms/SimulatedAnnealing.hpp>

#include <fstream>


namespace benchmark
{

void BenchmarkSA::Run()
{
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
            fileBench << "Duration of execution: " << result.duration_ << '\n';
            fileBench << "Execution iteration: " << result.iterations_ << '\n';
            fileBench << result.solution_ << '\n' << '\n';
        }
    }

    fileBench.close();
    
}


} // namespace benchmark
