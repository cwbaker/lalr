#ifndef SWEET_LALR_PARSERGENERATOR_HPP_INCLUDED
#define SWEET_LALR_PARSERGENERATOR_HPP_INCLUDED

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

class ParserErrorPolicy;
class ParserAction;
class ParserSymbol;
class ParserItem;
class ParserState;
class ParserProduction;
class ParserGrammar;
class GrammarParser;

/**
// @internal
//
// %Parser state machine generator.
*/
class ParserGenerator
{
    ParserErrorPolicy* error_policy_; ///< The event sink to report errors to and print with or null to ignore errors and prints.
    std::string identifier_; ///< The identifier of the parser.
    std::vector<std::unique_ptr<ParserAction>> actions_; ///< The actions in the parser.
    std::vector<std::unique_ptr<ParserProduction>> productions_; ///< The productions in the parser.
    std::vector<std::unique_ptr<ParserSymbol>> symbols_; ///< The symbols in the parser.
    std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>> states_; ///< The states in the parser's state machine.
    const ParserSymbol* start_symbol_; ///< The start symbol.
    const ParserSymbol* end_symbol_; ///< The end symbol.
    const ParserSymbol* error_symbol_; ///< The error symbol.
    ParserState* start_state_; ///< The start state.
    int errors_; ///< The number of errors that occured during parsing and generation.

    public:
        ParserGenerator( ParserGrammar& grammar, ParserErrorPolicy* error_policy );
        ~ParserGenerator();
        std::string& identifier();
        std::vector<std::unique_ptr<ParserAction> >& actions();
        std::vector<std::unique_ptr<ParserProduction> >& productions();
        std::vector<std::unique_ptr<ParserSymbol> >& symbols();
        std::set<std::shared_ptr<ParserState>, shared_ptr_less<ParserState>>& states();
        const ParserSymbol* start_symbol();
        const ParserSymbol* end_symbol();
        const ParserSymbol* error_symbol();
        ParserState* start_state();
        int errors() const;

        void fire_error( int line, int error, const char* format, ... );
        void fire_printf( const char* format, ... ) const;
                
    private:
        void generate( ParserGrammar& grammar );
        std::set<const ParserSymbol*> lookahead( const ParserItem& item ) const;
        void closure( const std::shared_ptr<ParserState>& state );
        std::shared_ptr<ParserState> goto_( const std::shared_ptr<ParserState>& state, const ParserSymbol& symbol );
        int lookahead_closure( ParserState* state ) const;
        int lookahead_goto( ParserState* state ) const;
        void calculate_precedence_of_productions();
        void generate_states( const ParserSymbol* start_symbol, const ParserSymbol* end_symbol, const std::vector<std::unique_ptr<ParserSymbol>>& symbols );
        void generate_indices_for_states();
        void generate_reduce_transitions();
        void generate_reduce_transition( ParserState* state, const ParserSymbol* symbol, const ParserProduction* production );
        void generate_indices_for_transitions();
};

}

}

#endif