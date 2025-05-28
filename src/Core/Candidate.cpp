#include <Core/Candidate.hpp>

#include <fstream>
#include <random>

namespace model
{

Candidate::Candidate(const std::size_t kFunctionLength) : quality_(0.0)
{
    this->function_ = GenerateRandomBooleanFunction(kFunctionLength);

    std::ofstream candidate_out;
    candidate_out.open("candidate.txt");

    if (candidate_out.is_open())
        candidate_out << this->function_;
    

    candidate_out.close();
}

Candidate::Candidate(const std::string& kString)
{
    this->function_ = kString;
}

Candidate::Candidate(std::string&& string)
{
    this->function_ = std::move(string);
}

std::string& Candidate::GetFunction()
{
    return this->function_;
}

void Candidate::SetFunction(const std::string& kFunction)
{
    this->function_ = kFunction;
}

double Candidate::GetQuality() const
{
    return this->quality_;
}

void Candidate::EvaluateQualityFunction(const CNF& kCNF)
{
    auto disjuncts = kCNF.SplitCNF();
    size_t quality_counter = 0;

    for (const auto &disjunct : disjuncts)
        if (EvaluateDisjunct(disjunct))
            ++quality_counter;

    this->quality_ = static_cast<double>(quality_counter) / disjuncts.size();
}

std::string Candidate::GenerateRandomBooleanFunction(const std::size_t kLength) const
{
    if (kLength == 0 || kLength > 64)
        throw std::invalid_argument("Function length must be between 1 and 64");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution distrib(0.5); // 50% chance for 0 or 1

    std::string function;
    function.resize(kLength);

    for (size_t i = 0; i < kLength; ++i)
        function[i] = distrib(gen) ? '1' : '0';

    return function;
}

bool Candidate::EvaluateDisjunct(const std::string& kDisjunct) const
{
    if (kDisjunct.empty() || this->function_.empty())
        return false;

    size_t pos = 0;
    while (pos < kDisjunct.length())
    {
        while (pos < kDisjunct.length() && kDisjunct[pos] == ' ')
            pos++;
        if (pos >= kDisjunct.length())
            break;

        bool negated = false;
        char var_char = kDisjunct[pos];

        if (var_char == '!')
        {
            negated = true;
            pos++;
            if (pos >= kDisjunct.length())
                break;
            var_char = kDisjunct[pos];
        }

        if (var_char != 'x' && !isdigit(var_char))
        {
            pos++;
            continue;
        }

        int var_index = 0;
        if (var_char == 'x')
        {
            pos++;
            if (pos >= kDisjunct.length() || !isdigit(kDisjunct[pos]))
                continue;

            var_index = kDisjunct[pos] - '0' - 1;
        }
        else
            var_index = var_char - '0' - 1;

        if (var_index < 0 || var_index >= static_cast<int>(this->function_.length()))
        {
            pos++;
            continue;
        }

        bool var_value = (this->function_[var_index] == '1');

        if (negated)
            var_value = !var_value;
        if (var_value)
            return true;

        pos++;
    }

    return false;
}

} // namespace model
