#pragma once

#include <Benchmarks/BenchmarkBase.hpp>

#include <Utils/Utils.hpp>

namespace benchmark
{

class BenchmarkBH final : public BaseBenchmark<utils::BHExecutionResult>
{
public:
    utils::BHExecutionResult Run() override;
};

} // namespace benchmark