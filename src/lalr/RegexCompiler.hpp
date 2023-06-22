#ifndef LALR_LEXERALLOCATIONS_HPP_INCLUDED
#define LALR_LEXERALLOCATIONS_HPP_INCLUDED

#include "RegexToken.hpp"
#include <vector>
#include <memory>
#include <string>
#include <deque>

namespace lalr
{

class ErrorPolicy;
class LexerAction;
class LexerTransition;
class LexerState;
class LexerStateMachine;
class RegexGenerator;

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
    int compile( const std::string& regular_expression, void* symbol, ErrorPolicy* error_policy = nullptr );
    int compile( const std::vector<RegexToken>& tokens, ErrorPolicy* error_policy = nullptr );
    const char* add_string( const std::string& string );
    void set_actions( std::unique_ptr<LexerAction[]>& actions, int actions_size );
    void set_transitions( std::unique_ptr<LexerTransition[]>& transitions, int transitions_size );
    void set_states( std::unique_ptr<LexerState[]>& states, int states_size, const LexerState* start_state );
    void populate_lexer_state_machine( const RegexGenerator& generator );
    size_t strings_size() {return strings_.size();};
};

}

#endif
