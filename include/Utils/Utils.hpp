#pragma once

#include <vector>
#include <string>
#include <limits>

namespace utils
{

constexpr double EPSILON = std::numeric_limits<double>::epsilon() * 10;


struct GAExecutionResult
{
    std::size_t iterations_; 
    std::vector<double> best_qualities_; 
    std::string solution_;

    std::size_t duration_;
};

} // namespace utils
