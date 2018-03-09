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
    std::unique_ptr<LexerAction[]> allocated_actions_; ///< The actions in this LexerStateMachine.
    std::unique_ptr<LexerTransition[]> allocated_transitions_; ///< The transitions in this LexerStateMachine.
    std::unique_ptr<LexerState[]> allocated_states_; ///< The states in this LexerStateMachine.
    int actions_size_;
    int transitions_size_;
    int states_size_;
    const LexerAction* actions_;
    const LexerTransition* transitions_;
    const LexerState* states_;
    const LexerState* start_state_; ///< The starting state for the state machine.

public:
    LexerStateMachine();
    ~LexerStateMachine();
    int actions_size() const;
    int transitions_size() const;
    int states_size() const;
    const LexerAction* actions() const;
    const LexerTransition* transitions() const;
    const LexerState* states() const;
    const LexerState* start_state() const;
    const LexerState* whitespace_start_state() const;
    void set_actions( std::unique_ptr<LexerAction[]>& actions, int actions_size );
    void set_transitions( std::unique_ptr<LexerTransition[]>& transitions, int transitions_size );
    void set_states( std::unique_ptr<LexerState[]>& states, int states_size, const LexerState* start_state );
};

}

}

#endif