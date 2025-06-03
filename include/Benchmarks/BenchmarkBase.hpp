#pragma once

#include <Core/CNF.hpp>
#include <Core/Candidate.hpp>
#include <Core/Candidates.hpp>

#include <memory>

namespace benchmark
{

template< class ResultOfRun >
class BaseBenchmark 
{
public:
    virtual ~BaseBenchmark() = default;

    static std::unique_ptr<model::CNF> CreateTestCNF();
    static std::unique_ptr<model::Candidate> CreateTestCandidate();
    static std::unique_ptr<model::Candidates> CreateTestCandidates(const std::uint32_t kCount);

    virtual ResultOfRun Run() = 0;
};

template< class ResultOfRun >
std::unique_ptr<model::CNF> BaseBenchmark<ResultOfRun>::CreateTestCNF()
{
    return std::make_unique<model::CNF>(10);
}

template< class ResultOfRun >
std::unique_ptr<model::Candidate> BaseBenchmark<ResultOfRun>::CreateTestCandidate()
{
    return std::make_unique<model::Candidate>(10);
}

template< class ResultOfRun >
std::unique_ptr<model::Candidates> BaseBenchmark<ResultOfRun>::CreateTestCandidates(const std::uint32_t kCount)
{
    return std::make_unique<model::Candidates>(kCount, 10);
}
    
} // namespace benchmark


