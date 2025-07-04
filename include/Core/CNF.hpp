#pragma once

#include <string>
#include <vector>


namespace model
{

class CNF final
{
public:
    CNF() = default;
    explicit CNF( const std::uint32_t kAmountVariables );
    explicit CNF( const std::string& kCNF );
    explicit CNF( std::string&& cnf ) noexcept;
    ~CNF() = default;

    std::string& GetCNF();
    std::vector<std::string> SplitCNF() const;

private:
    std::string GenerateRandomCNF(
        const std::uint32_t kVariableNumber, 
        const std::uint32_t kClauseLength, 
        const std::uint32_t kMaxClauseLength
    ) const;

    std::string TrimAndClean( const std::string& kString ) const;

    std::string cnf_str_;
};



} // namespace model
