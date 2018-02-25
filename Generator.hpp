#ifndef SWEET_LALR_GENERATOR_HPP_INCLUDED
#define SWEET_LALR_GENERATOR_HPP_INCLUDED

#include "LexerToken.hpp"
#include "shared_ptr_less.hpp"
#include <memory>
#include <set>
#include <vector>
#include <string>

namespace sweet
{

namespace error
{

class Error;

}

namespace lalr
{

class LexerStateMachine;
class LexerErrorPolicy;

}

namespace lalr
{

class LexerErrorPolicy;
class ParserErrorPolicy;
class ParserStateMachine;
class Action;
class LalrSymbol;
class Item;
class LalrState;
class LalrProduction;
class Grammar;

/**
// @internal
//
// %Parser state machine generator.
*/
class Generator
{
    ParserErrorPolicy* error_policy_; ///< The event sink to report errors to and print with or null to ignore errors and prints.
    std::string identifier_; ///< The identifier of the parser.
    std::vector<std::unique_ptr<Action>> actions_; ///< The actions in the parser.
    std::vector<std::unique_ptr<LalrProduction>> productions_; ///< The productions in the parser.
    std::vector<std::unique_ptr<LalrSymbol>> symbols_; ///< The symbols in the parser.
    std::set<std::shared_ptr<LalrState>, shared_ptr_less<LalrState>> states_; ///< The states in the parser's state machine.
    LalrSymbol* start_symbol_; ///< The start symbol.
    LalrSymbol* end_symbol_; ///< The end symbol.
    LalrSymbol* error_symbol_; ///< The error symbol.
    LalrState* start_state_; ///< The start state.
    int errors_; ///< The number of errors that occured during parsing and generation.

    public:
        Generator( Grammar& grammar, ParserStateMachine* parser_state_machine, ParserErrorPolicy* error_policy, LexerErrorPolicy* lexer_error_policy );
        ~Generator();
        std::string& identifier();
        std::vector<std::unique_ptr<Action> >& actions();
        std::vector<std::unique_ptr<LalrProduction> >& productions();
        std::vector<std::unique_ptr<LalrSymbol> >& symbols();
        std::set<std::shared_ptr<LalrState>, shared_ptr_less<LalrState>>& states();
        const LalrSymbol* start_symbol();
        const LalrSymbol* end_symbol();
        const LalrSymbol* error_symbol();
        LalrState* start_state();
        int errors() const;
                
    private:
        void fire_error( int line, int error, const char* format, ... );
        void fire_printf( const char* format, ... ) const;
        void generate( Grammar& grammar, ParserStateMachine* parser_state_machine, LexerErrorPolicy* lexer_error_policy );
        std::set<const LalrSymbol*> lookahead( const Item& item ) const;
        void closure( const std::shared_ptr<LalrState>& state );
        std::shared_ptr<LalrState> goto_( const std::shared_ptr<LalrState>& state, const LalrSymbol& symbol );
        int lookahead_closure( LalrState* state ) const;
        int lookahead_goto( LalrState* state ) const;
        void replace_references_to_symbol( LalrSymbol* to_symbol, LalrSymbol* with_symbol );
        void check_for_undefined_symbol_errors();
        void check_for_unreferenced_symbol_errors();
        void check_for_error_symbol_on_left_hand_side_errors();
        void calculate_identifiers();
        void calculate_terminal_and_non_terminal_symbols();
        void calculate_implicit_terminal_symbols();
        void calculate_precedence_of_productions();
        void calculate_symbol_indices();
        void calculate_first();
        void calculate_follow();
        void generate_states( const LalrSymbol* start_symbol, const LalrSymbol* end_symbol, const std::vector<std::unique_ptr<LalrSymbol>>& symbols );
        void generate_indices_for_states();
        void generate_reduce_transitions();
        void generate_reduce_transition( LalrState* state, const LalrSymbol* symbol, const LalrProduction* production );
        void generate_indices_for_transitions();
        void populate_parser_state_machine( const std::vector<LexerToken>& whitespace_tokens, ParserStateMachine* parser_state_machine, LexerErrorPolicy* lexer_error_policy );
};

}

}

#endif