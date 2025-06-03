#pragma once

#include <Benchmarks/BenchmarkBase.hpp>

#include <Utils/Utils.hpp>

namespace benchmark
{

class BenchmarkGA final : public BaseBenchmark<utils::GAExecutionResult>
{
public:
    utils::GAExecutionResult Run() override;
};

} // namespace benchmark
