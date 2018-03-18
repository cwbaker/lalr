#ifndef SWEET_LALR_GRAMMARSYMBOL_HPP_INCLUDED
#define SWEET_LALR_GRAMMARSYMBOL_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include <string>
#include <vector>
#include <set>

namespace sweet
{
    
namespace lalr
{

class GrammarProduction;

class GrammarSymbol
{
    std::string lexeme_;
    std::string identifier_; ///< The identifier for this symbol (generated from its lexeme).
    SymbolType symbol_type_;
    LexemeType lexeme_type_;
    Associativity associativity_;
    int precedence_;
    int line_;
    int index_;
    bool nullable_; ///< True if this symbol is nullable otherwise false.
    std::set<const GrammarSymbol*> first_; ///< The symbols that can start this symbol in a production or regular expression.
    std::set<const GrammarSymbol*> follow_; ///< The symbols that can follow this symbol in a production or regular expression.
    std::vector<GrammarProduction*> productions_; ///< The productions that reduce to this symbol.

public:
    GrammarSymbol( const std::string& lexeme );

    const std::string& lexeme() const;
    const std::string& identifier() const;
    SymbolType symbol_type() const;
    LexemeType lexeme_type() const;
    Associativity associativity() const;
    int precedence() const;
    int line() const;
    int index() const;
    bool nullable() const;
    const std::set<const GrammarSymbol*>& first() const;
    const std::set<const GrammarSymbol*>& follow() const;
    const std::vector<GrammarProduction*>& productions() const;
    GrammarSymbol* implicit_terminal() const;

    void set_lexeme( const std::string& lexeme );
    void set_identifier( const std::string& identifier );
    void set_symbol_type( SymbolType symbol_type );
    void set_lexeme_type( LexemeType lexeme_type );
    void set_associativity( Associativity associativity );
    void set_precedence( int precedence );
    void set_line( int line );
    void set_index( int index );
    void set_nullable( bool nullable );
    void append_production( GrammarProduction* production );
    void calculate_identifier();
    void replace_by_non_terminal( const GrammarSymbol* non_terminal_symbol );    
    int add_symbol_to_first( const GrammarSymbol* symbol );
    int add_symbols_to_first( const std::set<const GrammarSymbol*>& symbols );
    int add_symbol_to_follow( const GrammarSymbol* symbol );
    int add_symbols_to_follow( const std::set<const GrammarSymbol*>& symbols );
    int calculate_first();
    int calculate_follow();
};

}

}

#endif
