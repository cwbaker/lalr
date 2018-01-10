#ifndef SWEET_PARSER_PARSERGRAMMAR_HPP_INCLUDED
#define SWEET_PARSER_PARSERGRAMMAR_HPP_INCLUDED

#include "ParserSymbol.hpp"
#include <memory>
#include <vector>
#include <set>
#include <string>

namespace sweet
{

namespace parser
{

class ParserAction;
class ParserProduction;
class ParserGenerator;
class GrammarNode;

/**
// A grammar used to generate a state machine for a %parser.
*/
class ParserGrammar
{
    std::string identifier_; ///< The identifier of the grammar.
    std::vector<std::shared_ptr<ParserAction>> actions_; ///< The actions in the grammar.
    std::vector<std::shared_ptr<ParserProduction>> productions_; ///< The productions in the grammar.
    std::vector<std::shared_ptr<ParserSymbol>> symbols_; ///< The symbols in the grammar.
    ParserSymbol* start_symbol_; ///< The start symbol.
    ParserSymbol* end_symbol_; ///< The end symbol.
    ParserSymbol* error_symbol_; ///< The error symbol.

public:
    ParserGrammar( size_t actions_reserve = 32, size_t productions_reserve = 64, size_t symbols_reserve = 64 );

    std::string& identifier();
    std::vector<std::shared_ptr<ParserAction>>& actions();
    std::vector<std::shared_ptr<ParserProduction>>& productions();
    std::vector<std::shared_ptr<ParserSymbol>>& symbols();
    ParserSymbol* start_symbol() const;
    ParserSymbol* end_symbol() const;
    ParserSymbol* error_symbol() const;

    ParserSymbol* add_symbol( SymbolType type, const std::string& identifier, int line );
    ParserSymbol* add_terminal( const std::string& identifier, int line );
    ParserSymbol* add_non_terminal( const std::string& identifier, int line );
    ParserAction* add_action( const std::string& identifier );

    void identifier( const std::string& identifier );
    void begin_production( ParserSymbol* symbol, int line );
    void end_production();
    void symbol( ParserSymbol* symbol );
    void action( ParserAction* action );
    void precedence_symbol( ParserSymbol* symbol );

    void print() const;
    void print_positions( const std::set<int>& positions ) const;
    void calculate_identifiers();
    void replace_references_to_symbol( ParserSymbol* to_symbol, ParserSymbol* with_symbol );
    void calculate_implicit_terminal_symbols();
    void calculate_first();
    void calculate_follow();
    void calculate_indices();
    void calculate_precedence_of_productions();        
    void check_for_undefined_symbol_errors( ParserGenerator* generator );
    void check_for_unreferenced_symbol_errors( ParserGenerator* generator );
    void check_for_error_symbol_on_left_hand_side_errors( ParserGenerator* generator );
};

}

}

#endif