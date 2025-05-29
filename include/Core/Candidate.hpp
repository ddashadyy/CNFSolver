#pragma once

#include <string>
#include <Core/CNF.hpp>


namespace model
{

class Candidate final
{
public:
    Candidate() = default;
    
    explicit Candidate( const std::uint32_t kFunctionLength );
    Candidate( const std::string& kString );
    Candidate( std::string&& string );

    ~Candidate() = default;

    std::string& GetFunction();
    void SetFunction(const std::string& kFunction);
    double GetQuality() const;
    void EvaluateQualityFunction(const CNF& kCNF);

private:
    std::string GenerateRandomBooleanFunction(const std::uint32_t kLength) const;
    bool EvaluateDisjunct(const std::string& kDisjunct) const;

    std::string function_;
    double quality_;
};


} // namespace model
