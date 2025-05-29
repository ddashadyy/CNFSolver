#pragma once

#include <vector>
#include <string>
#include <limits>

namespace utils
{

constexpr double EPSILON = std::numeric_limits<double>::epsilon() * 10;


struct GAExecutionResult
{
    std::uint32_t iterations_; 
    std::vector<double> best_qualities_; 
    std::string solution_;

    std::uint32_t duration_;
};

} // namespace utils
