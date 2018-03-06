#ifndef SWEET_LEXER_LEXERSTATEMACHINE_HPP_INCLUDED
#define SWEET_LEXER_LEXERSTATEMACHINE_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>
#include "LexerToken.hpp"

namespace sweet
{

namespace lalr
{

class RegexAction;
class RegexState;
class LexerErrorPolicy;

/**
// The data that defines the state machine for a lexical analyzer.
*/
class LexerStateMachine
{
    std::string identifier_; ///< The identifier of this LexerStateMachine.
    std::vector<std::unique_ptr<RegexAction>> actions_; ///< The lexer actions for this ParserStateMachine.
    std::vector<std::shared_ptr<RegexState> > states_; ///< The states that make up the state machine for this LexerStateMachine.
    std::vector<std::shared_ptr<RegexState> > whitespace_states_; ///< The states that make up the state machine for whitespace in this LexerStateMachine.
    const RegexState* start_state_; ///< The starting state for the state machine.
    const RegexState* whitespace_start_state_; ///< The starting state for the whitespace state machine.

    public:
        LexerStateMachine( const std::string& regular_expression, void* symbol, LexerErrorPolicy* event_sink = NULL );
        LexerStateMachine( const std::string& identifier, const std::vector<LexerToken>& tokens, const std::vector<LexerToken>& whitespace_tokens = std::vector<LexerToken>(), LexerErrorPolicy* event_sink = NULL );
        ~LexerStateMachine();
        const std::string& identifier() const;
        const std::vector<std::unique_ptr<RegexAction>>& actions() const;
        const std::vector<std::shared_ptr<RegexState> >& states() const;
        const std::vector<std::shared_ptr<RegexState> >& whitespace_states() const;
        const RegexState* start_state() const;
        const RegexState* whitespace_start_state() const;
        void describe( std::string* description ) const;
        std::string description() const;
};

}

}

#endif