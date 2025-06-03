#pragma once

#include <vector>
#include <string>
#include <limits>
#include <optional>


namespace utils
{

constexpr double EPSILON = std::numeric_limits<double>::epsilon() * 10;


struct GAExecutionResult 
{
    std::uint32_t iterations;

    std::vector<double> best_qualities;         
    std::vector<double> best_progressive_qualities; 

    std::string solution;

    std::uint32_t duration;
};

struct SAExecutionResult final
{
    std::uint32_t iterations;
    
    std::vector<double> best_energies;
    std::vector<double> temperatures;
    std::vector<double> best_progressive_qualities;

    std::string solution;

    std::uint32_t duration;
};

struct BHExecutionResult final
{
    std::uint32_t iterations;

    std::vector<double> best_qualities;
    std::vector<double> best_progressive_qualities;

    std::string solution;

    std::uint32_t duration;
};

struct GraphData final
{
    std::vector<double> data_;
    std::wstring title_;
    double y_max_;

};

bool DoubleEqual(const double lhs, const double rhs);

bool DoubleLess(const double lhs, const double rhs);
bool DoubleGreater(const double lhs, const double rhs);
bool DoubleLessOrEqual(const double lhs, const double rhs);
bool DoubleGreaterOrEqual(const double lhs, const double rhs);


} // namespace utils
