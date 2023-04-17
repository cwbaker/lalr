#ifndef LALR_GRAMMARTRANSITION_HPP_INCLUDED
#define LALR_GRAMMARTRANSITION_HPP_INCLUDED

#include "TransitionType.hpp"
#include <string>
#include <set>

namespace lalr
{

class GrammarState;
class GrammarSymbol;
class GrammarProduction;

/**
// A transition in a parser's state machine.
*/
class GrammarTransition
{
    const GrammarSymbol* symbol_; ///< The symbol that the transition is taken on.
    mutable GrammarState* state_; ///< The state that is transitioned to.
    mutable const GrammarProduction* production_; /// The production reduced to or null for shifts

public:
    GrammarTransition( const GrammarSymbol* symbol, const GrammarProduction* production );
    GrammarTransition( const GrammarSymbol* symbol, GrammarState* state );
    GrammarState* state() const;
    const GrammarProduction* production() const;
    bool is_shift() const;
    bool is_reduce() const;
    const GrammarSymbol* reduced_symbol() const;
    int reduced_length() const;
    int precedence() const;
    int action() const;
    bool taken_on_symbol( const GrammarSymbol* symbol ) const;
    const GrammarSymbol* symbol() const;

    void override_shift_to_reduce( const GrammarProduction* production ) const;
    void override_reduce_to_reduce( const GrammarProduction* production ) const;
};

}

#endif
