#include <Core/CNF.hpp>

#include <fstream>
#include <sstream>
#include <random>


namespace model
{

CNF::CNF(const std::uint32_t kAmountVariables)
{
    std::ofstream cnf_out;
    cnf_out.open("cnf.txt");

    if (cnf_out.is_open())
    {
        auto cnf = GenerateRandomCNF(kAmountVariables, kAmountVariables * kAmountVariables, kAmountVariables + kAmountVariables);
        cnf_out << cnf;

        this->cnf_str_ = cnf;
    }

    cnf_out.close();
}

CNF::CNF(const std::string& kCNF)
    : cnf_str_(kCNF) {}

CNF::CNF(std::string&& cnf) noexcept
: cnf_str_(std::move(cnf)) {}

std::string &CNF::GetCNF()
{
    return this->cnf_str_;
}

std::vector<std::string> CNF::SplitCNF() const
{
    std::vector<std::string> result;
    if (this->cnf_str_.empty())
        return std::vector<std::string>{"empty"};

    const char delimiter = '&';
    std::istringstream iss(this->cnf_str_);
    std::string token;

    while (std::getline(iss, token, delimiter))
    {
        std::string cleaned = TrimAndCLean(token);
        if (!cleaned.empty())
            result.push_back(cleaned);
    }

    if (result.empty())
    {
        std::string cleaned = TrimAndCLean(this->cnf_str_);
        if (!cleaned.empty())
            result.push_back(cleaned);
    }

    return result;
}

std::string CNF::GenerateRandomCNF(
    const std::uint32_t kVariableNumber, 
    const std::uint32_t kClauseLength, 
    const std::uint32_t kMaxClauseLength
) const
{
    if (kVariableNumber == 0 || kClauseLength == 0 || kMaxClauseLength == 0)
        return "";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint32_t> clause_len_dist(1, kMaxClauseLength);
    std::uniform_int_distribution<std::uint32_t> var_dist(0, kVariableNumber - 1);
    std::bernoulli_distribution neg_dist(0.33); 

    std::vector<std::string> variables;
    for (std::uint32_t i = 0; i < kVariableNumber; ++i)
        variables.push_back("x" + std::to_string(i + 1));

    std::string cnf_str;

    for (std::uint32_t i = 0; i < kClauseLength; ++i)
    {
        cnf_str += "(";

        std::uint32_t clause_len = clause_len_dist(gen);
        std::vector<bool> used_vars(kVariableNumber, false);
        std::uint32_t vars_in_clause = 0;

        for (std::uint32_t j = 0; j < clause_len; ++j)
        {
            std::uint32_t var_idx;
            std::uint32_t attempts = 0;

            do
            {
                var_idx = var_dist(gen);
                attempts++;

                if (attempts > kVariableNumber * 2)
                    break;

            } while (used_vars[var_idx] && clause_len > 1);

            if (attempts > kVariableNumber * 2)
                break;

            used_vars[var_idx] = true;
            vars_in_clause++;

            if (neg_dist(gen))
                cnf_str += "!";

            cnf_str += variables[var_idx];

            if (j < clause_len - 1 && vars_in_clause < clause_len)
                cnf_str += " | ";
        }

        if (cnf_str.size() >= 3 && cnf_str.substr(cnf_str.size() - 3) == " | ")
            cnf_str.erase(cnf_str.size() - 3);

        cnf_str += ")";

        if (i < kClauseLength - 1)
            cnf_str += " & ";
    }

    return cnf_str;
}


std::string CNF::TrimAndCLean(const std::string& kString) const
{
    if (kString.empty())
        return kString;

    std::size_t start = kString.find_first_not_of(" \t()");
    if (start == std::string::npos)
        return "";

    std::size_t end = kString.find_last_not_of(" \t()");
    return kString.substr(start, end - start + 1);
}

} // namespace model
