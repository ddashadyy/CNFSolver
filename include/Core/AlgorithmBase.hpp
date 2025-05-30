#pragma once


#include <Core/CNF.hpp>

namespace algorithm
{

template< class ExecutionResul, class... Args >
class AlgorithmBase
{
public:

    AlgorithmBase() = default;
    AlgorithmBase( const model::CNF& kCNF );
    AlgorithmBase( model::CNF&& cnf ) noexcept;

    virtual ~AlgorithmBase() = default;

    virtual const ExecutionResul Execute( const Args... args ) = 0;


protected:
    model::CNF cnf_;
};

template< class ExecutionResul, class... Args >
AlgorithmBase<ExecutionResul, Args...>::AlgorithmBase(const model::CNF& kCNF) 
    : cnf_(kCNF) {}

template< class ExecutionResul, class... Args >
AlgorithmBase<ExecutionResul, Args...>::AlgorithmBase(model::CNF&& cnf) noexcept
    : cnf_(std::move(cnf)) {}
    
} // namespace algorithm
