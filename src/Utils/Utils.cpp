#include <Utils/Utils.hpp>

#include <cmath>

namespace utils
{

bool DoubleEqual(const double lhs, const double rhs)
{
    return std::fabs(lhs - rhs) <= EPSILON;
}

bool DoubleLess(const double lhs, const double rhs)
{
    return lhs < rhs - EPSILON;
}

bool DoubleGreater(const double lhs, const double rhs)
{
    return lhs > rhs + EPSILON;
}

bool DoubleLessOrEqual(const double lhs, const double rhs)
{
    return lhs <= rhs + EPSILON;
}

bool DoubleGreaterOrEqual(const double lhs, const double rhs)
{
    return lhs >= rhs - EPSILON;
}

} // namespace utils
