#include <Benchmarks/BenchmarkBase.hpp>




namespace benchmark
{

std::unique_ptr<model::CNF> benchmark::BaseBenchmark::CreateTestCNF()
{
    return std::make_unique<model::CNF>(10);
}

std::unique_ptr<model::Candidate> BaseBenchmark::CreateTestCandidate()
{
    return std::make_unique<model::Candidate>(10);
}

std::unique_ptr<model::Candidates> benchmark::BaseBenchmark::CreateTestCandidates(const std::uint32_t kCount)
{
    return std::make_unique<model::Candidates>(kCount, 10);
}


} // namespace benchmark


