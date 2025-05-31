#pragma once

#include <vector>
#include <string>
#include <limits>


namespace utils
{

constexpr double EPSILON = std::numeric_limits<double>::epsilon() * 10;


struct GAExecutionResult final
{
    std::uint32_t iterations_; 

    std::vector<double> best_qualities_; 
    std::string solution_;

    std::uint32_t duration_;
};

struct SAExecutionResult final
{
    std::uint32_t iterations_;
    
    std::vector<double> best_energies_;
    std::vector<double> temperatures_;

    std::string solution_;

    std::uint32_t duration_;
};

struct BHExecutionResult final
{
    std::uint32_t iterations_;

    std::vector<double> best_qualities_;
    std::string solution_;

    std::uint32_t duration_;
};

struct GraphData final
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
