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

struct SAExecutionResult
{
    std::uint32_t iterations_;
    
    std::vector<double> best_energies_;
    std::vector<double> temperatures_;

    std::string solution_;

    std::uint32_t duration_;
};

struct GraphData
{
    std::vector<double> data_;
    std::wstring title_;
};

bool DoubleEqual(const double lhs, const double rhs);

bool DoubleLess(const double lhs, const double rhs);
bool DoubleGreater(const double lhs, const double rhs);
bool DoubleLessOrEqual(const double lhs, const double rhs);
bool DoubleGreaterOrEqual(const double lhs, const double rhs);


} // namespace utils
