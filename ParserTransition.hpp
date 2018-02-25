#ifndef SWEET_LALR_PARSERTRANSITION_HPP_INCLUDED
#define SWEET_LALR_PARSERTRANSITION_HPP_INCLUDED

#include "TransitionType.hpp"

namespace sweet
{

namespace lalr
{

class ParserState;
class ParserSymbol;

/**
// A transition in a parser's state machine.
*/
class ParserTransition
{
public:
    const ParserSymbol* symbol; ///< The symbol that the transition is taken on.
    const ParserState* state; ///< The state that is transitioned to.
    const ParserSymbol* reduced_symbol; ///< The symbol that is reduced to or null if this isn't a reducing transition.
    int reduced_length; ///< The number of symbols on the right-hand side of the reduced production.
    int precedence; ///< The precedence of the reduce production or 0 for the default precedence or no reduction.
    int action; ///< The index of the action taken on reduce or Action::INVALID_INDEX if there is no action associated with the reduce.
    TransitionType type; ///< The type of transition that this is.
    int index; ///< The index of this transition.

public:
    ParserTransition();
};

}

}

#endif
