#pragma once

#include <string>
#include <vector>


namespace model
{

class CNF final
{
public:
    CNF() = default;
    explicit CNF( const std::size_t kAmountVariables );
    ~CNF() = default;

    std::string& GetCNF();
    std::vector<std::string> SplitCNF() const;

private:
    std::string GenerateRandomCNF(
        const std::size_t kVariableNumber, 
        const std::size_t kClauseLength, 
        const std::size_t kMaxClauseLength
    ) const;

    std::string TrimAndCLean( const std::string& kString ) const;

    std::string cnf_str_;
};



} // namespace model
