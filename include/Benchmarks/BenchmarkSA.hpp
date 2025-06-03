#pragma once

#include <Benchmarks/BenchmarkBase.hpp>

#include <Utils/Utils.hpp>

namespace benchmark
{

class BenchmarkSA final : public BaseBenchmark<utils::SAExecutionResult>
{
public:
    utils::SAExecutionResult Run() override;
};

} // namespace benchmark
