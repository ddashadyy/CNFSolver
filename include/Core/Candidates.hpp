#pragma once


#include <Core/Candidate.hpp>


namespace model
{

class Candidates final
{
public:
    Candidates() = default;
    explicit Candidates(
        const std::size_t kAmountCandidates,
        const std::size_t kFunctionLength
    );

    ~Candidates() = default;

    std::vector<Candidate>& GetCandidates();

private:
    std::vector<Candidate> solution_candidates_;
};

} // namespace model
