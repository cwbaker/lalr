#ifndef LALR_GRAMMARGENERATOR_HPP_INCLUDED
#define LALR_GRAMMARGENERATOR_HPP_INCLUDED

#include "RegexToken.hpp"
#include "GrammarStateLess.hpp"
#include "GrammarProductionLess.hpp"
#include "GrammarSymbolSet.hpp"
#include "GrammarLookahead.hpp"
#include <memory>
#include <set>
#include <vector>
#include <string>

namespace lalr
{

class ErrorPolicy;
#ifndef LALR_NO_THREADS
class ThreadPool;
#endif
class LexerStateMachine;
class GrammarCompiler;
class GrammarAction;
class GrammarSymbol;
class GrammarTransition;
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
#ifndef LALR_NO_THREADS
    ThreadPool* thread_pool_; ///< The pool of threads to use to generate the parser.
#endif
    std::string identifier_; ///< The identifier of the parser.
    std::vector<std::unique_ptr<GrammarAction>> actions_; ///< The actions in the parser.
    std::vector<std::unique_ptr<GrammarProduction>> productions_; ///< The productions in the parser.
    std::vector<std::unique_ptr<GrammarSymbol>> symbols_; ///< The symbols in the parser.
    std::set<std::shared_ptr<GrammarState>, GrammarStateLess> states_; ///< The states in the parser's state machine.
    std::vector<std::unique_ptr<GrammarTransition>> transitions_; ///< The transitions in the parser.
    std::vector<GrammarLookahead> lookaheads_; // Lookaheads for each item in each state.
    GrammarSymbol* start_symbol_; ///< The start symbol.
    GrammarSymbol* end_symbol_; ///< The end symbol.
    GrammarSymbol* error_symbol_; ///< The error symbol.
    GrammarSymbol* whitespace_symbol_; ///< The whitespace symbol.
    GrammarState* start_state_; ///< The start state.
    int errors_; ///< The number of errors that occured during parsing and generation.
    int shift_reduce_count_; ///< The number of shift/reduce resolved conflicts that occured during parsing and generation.
    int reduce_reduce_count_; ///< The number of reduce/reduce resolved conflicts that occured during parsing and generation.
    bool error_recovery_off_; ///< True iff we want to turn off error recovery for debug.
    bool error_recovery_show_; ///< True iff we want to show error recovery for debug.

public:
    GrammarGenerator();
    ~GrammarGenerator();
    const std::vector<std::unique_ptr<GrammarAction>>& actions() const;
    const std::vector<std::unique_ptr<GrammarSymbol>>& symbols() const;
    const std::set<std::shared_ptr<GrammarState>, GrammarStateLess>& states() const;
    const std::vector<std::unique_ptr<GrammarTransition>>& transitions() const;
    const GrammarState* start_state() const;
    const GrammarSymbol* whitespace_symbol() const;
    std::string label_state( const GrammarState& state ) const;
    std::string label_item( const GrammarItem& item ) const;
    int generate( Grammar& grammar, ErrorPolicy* error_policy );
    int shift_reduce_count() const {return shift_reduce_count_;}
    int reduce_reduce_count() const {return reduce_reduce_count_;}
    bool is_error_recovery_off() const {return error_recovery_off_;}
    bool is_error_recovery_show() const {return error_recovery_show_;}

private:
    void error( int line, int column, int error, const char* format, ... );
    GrammarTransition* shift_transition( const GrammarSymbol* symbol, GrammarState* state );
    GrammarTransition* reduce_transition( const GrammarSymbol* symbol, const GrammarProduction* production );
    GrammarSymbolSet spontaneous_lookaheads( const GrammarItem& item ) const;
    void closure( const std::shared_ptr<GrammarState>& state );
    std::shared_ptr<GrammarState> goto_( const GrammarState* state, const GrammarSymbol& symbol );
    void replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol );
    void check_for_undefined_symbol_errors();
    void check_for_unreferenced_symbol_errors();
    void check_for_error_symbol_on_left_hand_side_errors();
    void check_for_implicit_terminal_duplicate_associativity();
    void calculate_identifiers();
    void calculate_terminal_and_non_terminal_symbols();
    void calculate_implicit_terminal_symbols();
    void calculate_precedence_of_productions();
    void calculate_symbol_indices();
    void calculate_first();
    void calculate_follow();
    void calculate_reachable_productions();
    void calculate_reachable_productions_for_symbol( const GrammarSymbol& symbol, std::set<GrammarProduction*, GrammarProductionLess>* productions );
    void generate_spontaneous_lookaheads();
    void generate_goto_items();
    void generate_propagated_lookaheads();
    void generate_states( const GrammarSymbol* start_symbol, const GrammarSymbol* end_symbol, const std::vector<std::unique_ptr<GrammarSymbol>>& symbols );
    void generate_indices_for_states();
    void generate_reduce_transitions();
    void generate_reduce_transition( GrammarState* state, const GrammarSymbol* symbol, const GrammarProduction* production, const GrammarProduction* curr_production );
    void generate_indices_for_transitions();
};

}

#endif
