#pragma once


#include <Core/CNF.hpp>

#include <memory>

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
    std::unique_ptr<model::CNF> cnf_;
};

template< class ExecutionResul, class... Args >
AlgorithmBase<ExecutionResul, Args...>::AlgorithmBase(const model::CNF& kCNF) 
    : cnf_(std::make_unique<model::CNF>(kCNF)) {}

template< class ExecutionResul, class... Args >
AlgorithmBase<ExecutionResul, Args...>::AlgorithmBase(model::CNF&& cnf) noexcept
    : cnf_(std::make_unique<model::CNF>(std::move(cnf))) {}
    
} // namespace algorithm
