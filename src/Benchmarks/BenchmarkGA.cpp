#include <Benchmarks/BenchmarkGA.hpp>
#include <Algorithms/GeneticAlgorithm.hpp>

#include <fstream>


namespace benchmark
{

void BenchmarkGA::Run()
{
    std::ofstream fileBench("GA_benchmark.txt");
    if (fileBench.is_open())
    {
        for (std::uint32_t i = 0; i < 10; i++)
        {
            const auto cnf = CreateTestCNF();
            const auto candidates = CreateTestCandidates(10);

            auto algorithm = std::make_unique<algorithm::GeneticAlgorithm>(*cnf, *candidates);

            const auto result = algorithm->Execute(
                1000,
                10,
                5,
                5,
                3,
                utils::selection_function::kExponential
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
