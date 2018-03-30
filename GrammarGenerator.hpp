#ifndef SWEET_LALR_GRAMMARGENERATOR_HPP_INCLUDED
#define SWEET_LALR_GRAMMARGENERATOR_HPP_INCLUDED

#include "RegexToken.hpp"
#include "GrammarSymbolLess.hpp"
#include "GrammarStateLess.hpp"
#include "shared_ptr_less.hpp"
#include <memory>
#include <set>
#include <vector>
#include <string>

namespace sweet
{

namespace lalr
{

class ErrorPolicy;
class LexerStateMachine;
class GrammarCompiler;
class GrammarAction;
class GrammarSymbol;
class GrammarItem;
class GrammarState;
class GrammarProduction;
class Grammar;

/**
// @internal
//
// %Parser state machine generator.
*/
class GrammarGenerator
{
    ErrorPolicy* error_policy_; ///< The event sink to report errors to and print with or null to ignore errors and prints.
    std::string identifier_; ///< The identifier of the parser.
    std::vector<std::unique_ptr<GrammarAction>> actions_; ///< The actions in the parser.
    std::vector<std::unique_ptr<GrammarProduction>> productions_; ///< The productions in the parser.
    std::vector<std::unique_ptr<GrammarSymbol>> symbols_; ///< The symbols in the parser.
    std::set<std::shared_ptr<GrammarState>, GrammarStateLess> states_; ///< The states in the parser's state machine.
    GrammarSymbol* start_symbol_; ///< The start symbol.
    GrammarSymbol* end_symbol_; ///< The end symbol.
    GrammarSymbol* error_symbol_; ///< The error symbol.
    GrammarState* start_state_; ///< The start state.
    int errors_; ///< The number of errors that occured during parsing and generation.

    public:
        GrammarGenerator();
        ~GrammarGenerator();
        const std::vector<std::unique_ptr<GrammarAction>>& actions() const;
        const std::vector<std::unique_ptr<GrammarSymbol>>& symbols() const;
        const std::set<std::shared_ptr<GrammarState>, GrammarStateLess>& states() const;
        const GrammarState* start_state() const;
        int generate( Grammar& grammar, GrammarCompiler* parser_allocations, ErrorPolicy* error_policy );
                
    private:
        void fire_error( int line, int error, const char* format, ... );
        void fire_printf( const char* format, ... ) const;
        std::set<const GrammarSymbol*, GrammarSymbolLess> lookahead( const GrammarItem& item ) const;
        void closure( const std::shared_ptr<GrammarState>& state );
        std::shared_ptr<GrammarState> goto_( const std::shared_ptr<GrammarState>& state, const GrammarSymbol& symbol );
        int lookahead_closure( GrammarState* state ) const;
        int lookahead_goto( GrammarState* state ) const;
        void replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol );
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
        void generate_states( const GrammarSymbol* start_symbol, const GrammarSymbol* end_symbol, const std::vector<std::unique_ptr<GrammarSymbol>>& symbols );
        void generate_indices_for_states();
        void generate_reduce_transitions();
        void generate_reduce_transition( GrammarState* state, const GrammarSymbol* symbol, const GrammarProduction* production );
        void generate_indices_for_transitions();
};

}

}

#endif