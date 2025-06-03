#pragma once

#include <Core/CNF.hpp>
#include <Core/Candidate.hpp>
#include <Core/Candidates.hpp>

#include <memory>

namespace benchmark
{

class BaseBenchmark 
{
public:
    virtual ~BaseBenchmark() = default;

    static std::unique_ptr<model::CNF> CreateTestCNF();
    static std::unique_ptr<model::Candidate> CreateTestCandidate();
    static std::unique_ptr<model::Candidates> CreateTestCandidates(const std::uint32_t kCount);

    virtual void Run() = 0;
};
    
} // namespace benchmark


