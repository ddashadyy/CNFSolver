#include <Core/Candidates.hpp>

#include <fstream>

namespace model
{

Candidates::Candidates(
    const std::size_t kAmountCandidates,
    const std::size_t kFunctionLength
)
{
    std::ofstream candidates_out("candidates.txt");

    solution_candidates_.reserve(kAmountCandidates);

    for (std::size_t i = 0; i < kAmountCandidates; i++)
    {
        solution_candidates_.emplace_back(Candidate(kFunctionLength));
        if (candidates_out.is_open())
            candidates_out << solution_candidates_[i].GetFunction() << '\n';
    }
}

std::vector<Candidate>& Candidates::GetCandidates()
{
    return this->solution_candidates_;
}

} // namespace model
