#ifndef SWEET_LALR_LEXERSTATEMACHINE_HPP_INCLUDED
#define SWEET_LALR_LEXERSTATEMACHINE_HPP_INCLUDED

#include <memory>

namespace sweet
{

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
    int actions_size;
    int transitions_size;
    int states_size;
    const LexerAction* actions;
    const LexerTransition* transitions;
    const LexerState* states;
    const LexerState* start_state;
};

}

}

#endif
