#include <Core/Candidates.hpp>

#include <fstream>


namespace model
{

Candidates::Candidates(
    const std::uint32_t kAmountCandidates,
    const std::uint32_t kFunctionLength
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

Candidates::Candidates(const std::vector<Candidate>& kCandidates)
    : solution_candidates_(kCandidates) {}

Candidates::Candidates(std::vector<Candidate> &&candidates) noexcept
    : solution_candidates_(std::move(candidates)) {}

std::vector<Candidate> &Candidates::GetCandidates()
{
    return this->solution_candidates_;
}

} // namespace model
