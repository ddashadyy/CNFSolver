#pragma once

#include <Benchmarks/BenchmarkBase.hpp>


namespace benchmark
{

class BenchmarkBH final : public BaseBenchmark
{
public:
    void Run() override;
};

} // namespace benchmark