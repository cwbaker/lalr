#ifndef SWEET_PARSER_PARSERSTATEMACHINE_HPP_INCLUDED
#define SWEET_PARSER_PARSERSTATEMACHINE_HPP_INCLUDED

#include <memory>
#include <vector>
#include <string>

namespace sweet
{

namespace lalr
{

class LexerStateMachine;
class LexerErrorPolicy;

}

namespace lalr
{

class Grammar;
class ParserGrammar;
class ParserAction;
class ParserProduction;
class ParserSymbol;
class ParserState;
class ParserErrorPolicy;

/**
// The data that defines the state machine for a %parser.
*/
class ParserStateMachine
{
    std::string identifier_; ///< The identifier of this ParserStateMachine.
    std::vector<std::shared_ptr<ParserAction> > actions_; ///< The parser actions for this ParserStateMachine.
    std::vector<std::shared_ptr<ParserProduction> > productions_; ///< The productions in the grammar for this ParserStateMachine.
    std::vector<std::shared_ptr<ParserSymbol> > symbols_; ///< The symbols in the grammar for this ParserStateMachine.
    std::vector<std::shared_ptr<ParserState> > states_; ///< The states in the state machine for this ParserStateMachine.
    const ParserSymbol* start_symbol_; ///< The start symbol.
    const ParserSymbol* end_symbol_; ///< The end symbol.
    const ParserSymbol* error_symbol_; ///< The error symbol.
    ParserState* start_state_; ///< The start state.
    std::shared_ptr<LexerStateMachine> lexer_state_machine_; ///< The LexerStateMachine that are used for lexical analysis in this ParserStateMachine.

    public:
        ParserStateMachine( Grammar& grammar, ParserErrorPolicy* error_policy = nullptr, LexerErrorPolicy* lexer_error_policy = nullptr );
        ParserStateMachine( ParserGrammar& grammar, ParserErrorPolicy* error_policy = nullptr );
        // ParserStateMachine( const char* filename, ParserErrorPolicy* error_policy = nullptr, lexer::LexerErrorPolicy* lexer_error_policy = nullptr );
        // ParserStateMachine( const char* start, const char* finish, ParserErrorPolicy* error_policy = nullptr, lexer::LexerErrorPolicy* lexer_error_policy = nullptr );

        const std::string& identifier() const;
        const std::vector<std::shared_ptr<ParserAction> >& actions() const;
        const std::vector<std::shared_ptr<ParserProduction> >& productions() const;        
        const std::vector<std::shared_ptr<ParserSymbol> >& symbols() const;
        const std::vector<std::shared_ptr<ParserState> >& states() const;
        const ParserSymbol* start_symbol() const;
        const ParserSymbol* end_symbol() const;
        const ParserSymbol* error_symbol() const;
        const ParserState* start_state() const;
        const LexerStateMachine* lexer_state_machine() const;
        const ParserSymbol* find_symbol_by_identifier( const char* identifier ) const;
        std::string description() const;
};

}

}

#endif
