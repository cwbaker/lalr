#ifndef SWEET_LALR_LEXERALLOCATIONS_HPP_INCLUDED
#define SWEET_LALR_LEXERALLOCATIONS_HPP_INCLUDED

#include <memory>
#include <string>
#include <deque>

namespace sweet
{

namespace lalr
{

class LexerAction;
class LexerTransition;
class LexerState;
class LexerStateMachine;

/**
// The data that defines the state machine for a lexical analyzer.
*/
class RegexCompiler
{
    std::deque<std::string> strings_;
    std::unique_ptr<LexerAction[]> actions_;
    std::unique_ptr<LexerTransition[]> transitions_;
    std::unique_ptr<LexerState[]> states_;
    std::unique_ptr<LexerStateMachine> state_machine_; 

public:
    RegexCompiler();
    ~RegexCompiler();
    const LexerStateMachine* state_machine() const;
    const char* add_string( const std::string& string );
    void set_actions( std::unique_ptr<LexerAction[]>& actions, int actions_size );
    void set_transitions( std::unique_ptr<LexerTransition[]>& transitions, int transitions_size );
    void set_states( std::unique_ptr<LexerState[]>& states, int states_size, const LexerState* start_state );
};

}

}

#endif
