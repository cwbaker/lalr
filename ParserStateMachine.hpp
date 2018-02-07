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
class LalrGrammar;
class LalrAction;
class LalrSymbol;
class LalrState;
class ParserErrorPolicy;

/**
// The data that defines the state machine for a %parser.
*/
class ParserStateMachine
{
    std::string identifier_; ///< The identifier of this ParserStateMachine.
    std::vector<std::unique_ptr<LalrAction> > actions_; ///< The parser actions for this ParserStateMachine.
    std::vector<std::unique_ptr<LalrSymbol> > symbols_; ///< The symbols in the grammar for this ParserStateMachine.
    std::vector<std::shared_ptr<LalrState> > states_; ///< The states in the state machine for this ParserStateMachine.
    const LalrSymbol* start_symbol_; ///< The start symbol.
    const LalrSymbol* end_symbol_; ///< The end symbol.
    const LalrSymbol* error_symbol_; ///< The error symbol.
    LalrState* start_state_; ///< The start state.
    std::shared_ptr<LexerStateMachine> lexer_state_machine_; ///< The LexerStateMachine that are used for lexical analysis in this ParserStateMachine.

    public:
        ParserStateMachine( Grammar& grammar, ParserErrorPolicy* error_policy = nullptr, LexerErrorPolicy* lexer_error_policy = nullptr );
        ParserStateMachine( LalrGrammar& grammar, ParserErrorPolicy* error_policy = nullptr );
        ~ParserStateMachine();
        const std::string& identifier() const;
        const std::vector<std::unique_ptr<LalrAction> >& actions() const;
        const std::vector<std::unique_ptr<LalrSymbol> >& symbols() const;
        const std::vector<std::shared_ptr<LalrState> >& states() const;
        const LalrSymbol* start_symbol() const;
        const LalrSymbol* end_symbol() const;
        const LalrSymbol* error_symbol() const;
        const LalrState* start_state() const;
        const LexerStateMachine* lexer_state_machine() const;
        const LalrSymbol* find_symbol_by_identifier( const char* identifier ) const;
        std::string description() const;
};

}

}

#endif
