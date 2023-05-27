#pragma once

namespace lalr
{

class GrammarProduction;

/**
// Indirectly compare productions through pointers.
*/
class GrammarProductionLess
{
public:
    inline bool operator()( const GrammarProduction* lhs, const GrammarProduction* rhs ) const;
};

}

#include "GrammarProductionLess.ipp"
