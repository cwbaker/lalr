//
// ParserStateMachine.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_PARSER_PARSERSTATEMACHINE_HPP_INCLUDED
#define SWEET_PARSER_PARSERSTATEMACHINE_HPP_INCLUDED

#include "declspec.hpp"
#include <sweet/pointer/ptr.hpp>
#include <vector>
#include <string>

namespace sweet
{

namespace lexer
{

class LexerStateMachine;
class LexerErrorPolicy;

}

namespace parser
{

class ParserGrammar;
class ParserAction;
class ParserProduction;
class ParserSymbol;
class ParserState;
class ParserErrorPolicy;

/**
// The data that defines the state machine for a %parser.
*/
class SWEET_PARSER_DECLSPEC ParserStateMachine
{
    std::string identifier_; ///< The identifier of this ParserStateMachine.
    std::vector<ptr<ParserAction> > actions_; ///< The parser actions for this ParserStateMachine.
    std::vector<ptr<ParserProduction> > productions_; ///< The productions in the grammar for this ParserStateMachine.
    std::vector<ptr<ParserSymbol> > symbols_; ///< The symbols in the grammar for this ParserStateMachine.
    std::vector<ptr<ParserState> > states_; ///< The states in the state machine for this ParserStateMachine.
    const ParserSymbol* start_symbol_; ///< The start symbol.
    const ParserSymbol* end_symbol_; ///< The end symbol.
    const ParserSymbol* error_symbol_; ///< The error symbol.
    ParserState* start_state_; ///< The start state.
    ptr<lexer::LexerStateMachine> lexer_state_machine_; ///< The LexerStateMachine that are used for lexical analysis in this ParserStateMachine.

    public:
        ParserStateMachine( ParserGrammar& grammar, ParserErrorPolicy* error_policy = NULL );
        ParserStateMachine( const char* filename, ParserErrorPolicy* error_policy = NULL, lexer::LexerErrorPolicy* lexer_error_policy = NULL );
        ParserStateMachine( const char* start, const char* finish, ParserErrorPolicy* error_policy = NULL, lexer::LexerErrorPolicy* lexer_error_policy = NULL );

        const std::string& identifier() const;
        const std::vector<ptr<ParserAction> >& actions() const;
        const std::vector<ptr<ParserProduction> >& productions() const;        
        const std::vector<ptr<ParserSymbol> >& symbols() const;
        const std::vector<ptr<ParserState> >& states() const;
        const ParserSymbol* start_symbol() const;
        const ParserSymbol* end_symbol() const;
        const ParserSymbol* error_symbol() const;
        const ParserState* start_state() const;
        const lexer::LexerStateMachine* lexer_state_machine() const;
        const ParserSymbol* find_symbol_by_identifier( const char* identifier ) const;
        std::string description() const;
};

}

}

#endif
