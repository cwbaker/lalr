#ifndef LALR_LEXERSTATEMACHINE_HPP_INCLUDED
#define LALR_LEXERSTATEMACHINE_HPP_INCLUDED

#include <memory>

namespace lalr
{

class LexerErrorPolicy;
class LexerAction;
class LexerTransition;
class LexerState;

/**
// The data that defines the state machine for a lexical analyzer.
*/
class LexerStateMachine
{
public:
    int actions_size; ///< The number of actions.
    int transitions_size; ///< The total number of transitions.
    int states_size; ///< The number of states.
    const LexerAction* actions; ///< The actions in this lexer.
    const LexerTransition* transitions; ///< The transitions in this lexer (for all states).
    const LexerState* states; ///< The states in this lexer.
    const LexerState* start_state; ///< The starting state in this lexer.
};

}

#endif
