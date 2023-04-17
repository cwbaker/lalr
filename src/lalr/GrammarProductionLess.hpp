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
    bool operator()( const GrammarProduction* lhs, const GrammarProduction* rhs ) const;
};

}
