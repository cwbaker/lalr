#ifndef SWEET_LEXER_LEXERSTATEMACHINE_HPP_INCLUDED
#define SWEET_LEXER_LEXERSTATEMACHINE_HPP_INCLUDED

#include <memory>
#include <string>
#include <vector>
#include "LexerToken.hpp"

namespace sweet
{

namespace parser
{

class LexerAction;
class LexerState;
class LexerErrorPolicy;

/**
// The data that defines the state machine for a lexical analyzer.
*/
class LexerStateMachine
{
    std::string identifier_; ///< The identifier of this LexerStateMachine.
    std::vector<std::shared_ptr<LexerAction> > actions_; ///< The lexer actions for this ParserStateMachine.
    std::vector<std::shared_ptr<LexerState> > states_; ///< The states that make up the state machine for this LexerStateMachine.
    std::vector<std::shared_ptr<LexerState> > whitespace_states_; ///< The states that make up the state machine for whitespace in this LexerStateMachine.
    const LexerState* start_state_; ///< The starting state for the state machine.
    const LexerState* whitespace_start_state_; ///< The starting state for the whitespace state machine.

    public:
        LexerStateMachine( const std::string& regular_expression, void* symbol, LexerErrorPolicy* event_sink = NULL );
        LexerStateMachine( const std::string& identifier, const std::vector<LexerToken>& tokens, const std::vector<LexerToken>& whitespace_tokens = std::vector<LexerToken>(), LexerErrorPolicy* event_sink = NULL );

        const std::string& identifier() const;
        const std::vector<std::shared_ptr<LexerAction> >& actions() const;
        const std::vector<std::shared_ptr<LexerState> >& states() const;
        const std::vector<std::shared_ptr<LexerState> >& whitespace_states() const;
        const LexerState* start_state() const;
        const LexerState* whitespace_start_state() const;
        void describe( std::string* description ) const;
        std::string description() const;
};

}

}

#endif