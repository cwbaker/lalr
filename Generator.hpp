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
class Symbol;
class Item;
class State;
class Production;
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
    std::vector<std::unique_ptr<Production>> productions_; ///< The productions in the parser.
    std::vector<std::unique_ptr<Symbol>> symbols_; ///< The symbols in the parser.
    std::set<std::shared_ptr<State>, shared_ptr_less<State>> states_; ///< The states in the parser's state machine.
    Symbol* start_symbol_; ///< The start symbol.
    Symbol* end_symbol_; ///< The end symbol.
    Symbol* error_symbol_; ///< The error symbol.
    State* start_state_; ///< The start state.
    int errors_; ///< The number of errors that occured during parsing and generation.

    public:
        Generator();
        ~Generator();
        int generate( Grammar& grammar, ParserStateMachine* parser_state_machine, ParserErrorPolicy* error_policy, LexerErrorPolicy* lexer_error_policy );
                
    private:
        void fire_error( int line, int error, const char* format, ... );
        void fire_printf( const char* format, ... ) const;
        std::set<const Symbol*> lookahead( const Item& item ) const;
        void closure( const std::shared_ptr<State>& state );
        std::shared_ptr<State> goto_( const std::shared_ptr<State>& state, const Symbol& symbol );
        int lookahead_closure( State* state ) const;
        int lookahead_goto( State* state ) const;
        void replace_references_to_symbol( Symbol* to_symbol, Symbol* with_symbol );
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
        void generate_states( const Symbol* start_symbol, const Symbol* end_symbol, const std::vector<std::unique_ptr<Symbol>>& symbols );
        void generate_indices_for_states();
        void generate_reduce_transitions();
        void generate_reduce_transition( State* state, const Symbol* symbol, const Production* production );
        void generate_indices_for_transitions();
        void populate_parser_state_machine( const Grammar& grammar, ParserStateMachine* parser_state_machine, LexerErrorPolicy* lexer_error_policy );
};

}

}

#endif