#pragma once


#include <Core/Candidate.hpp>


namespace model
{

class Candidates final
{
public:
    Candidates() = default;
    explicit Candidates(
        const std::uint32_t kAmountCandidates,
        const std::uint32_t kFunctionLength
    );

    explicit Candidates( const std::vector<Candidate>& kCandidates );
    explicit Candidates( std::vector<Candidate>&& candidates ) noexcept;

    ~Candidates() = default;

    std::vector<Candidate>& GetCandidates();

private:
    std::vector<Candidate> solution_candidates_;
};

} // namespace model
