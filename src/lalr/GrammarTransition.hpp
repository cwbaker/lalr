#ifndef LALR_GRAMMARTRANSITION_HPP_INCLUDED
#define LALR_GRAMMARTRANSITION_HPP_INCLUDED

#include "TransitionType.hpp"
#include "GrammarProduction.hpp"
#include <string>
#include <set>

namespace lalr
{

class GrammarState;
class GrammarSymbol;

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
/**
// Get the state that this transition is to.
//
// @return
//  The state or null if this transition is a reduction.
*/
    GrammarState* state() const { return state_;}
/**
// Get the production that this transition reduces.
//
// @return
//  The reduced production or null if this transition is a shift.
*/
    const GrammarProduction* production() const { return production_;}
    bool is_shift() const { return state_ != nullptr;}
    bool is_reduce() const { return production_ != nullptr;}
    const GrammarSymbol* reduced_symbol() const {return production_ ? production_->symbol() : nullptr;}
    int reduced_length() const {return production_ ? production_->length() : 0;}
    int precedence() const {return production_ ? production_->precedence() : 0;}
    int action() const {return production_ ? production_->action_index() : 0;}
/**
// Is this transition made on \e symbol?
//
// @param symbol
//  The symbol to check this transition being made on.
//
// @return
//  True if this transition is taken on \e symbol otherwise false.
*/
    bool taken_on_symbol( const GrammarSymbol* symbol ) const {return symbol_ == symbol;}
    const GrammarSymbol* symbol() const {return symbol_;}

    void override_shift_to_reduce( const GrammarProduction* production ) const;
    void override_reduce_to_reduce( const GrammarProduction* production ) const;
};

}

#endif
