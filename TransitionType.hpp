#ifndef SWEET_LALR_TRANSITIONTYPE_HPP_INCLUDED
#define SWEET_LALR_TRANSITIONTYPE_HPP_INCLUDED

namespace sweet
{

namespace lalr
{

/**
// The type of a transition in a parser's state machine.
*/
enum TransitionType
{
    TRANSITION_SHIFT, ///< Shift a terminal symbol onto the parser's stack.
    TRANSITION_REDUCE ///< Reduce one or more symbols on the parser's stack into a non terminal.
};

}

}

#endif
