#ifndef LALR_GRAMMARSYMBOL_HPP_INCLUDED
#define LALR_GRAMMARSYMBOL_HPP_INCLUDED

#include "GrammarSymbolLess.hpp"
#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include <string>
#include <vector>
#include <set>
#include <map>

namespace lalr
{

class GrammarProduction;

class GrammarSymbol
{
    std::string lexeme_; ///< The lexeme of this symbol.
    std::string identifier_; ///< The identifier for this symbol (generated from its lexeme).
    SymbolType symbol_type_; ///< The type of this symbol (terminal, non-terminal, or end).
    LexemeType lexeme_type_; ///< The type of this symbol's lexeme (regular expression or literal).
    Associativity associativity_; ///< The associativity of this symbol.
    int precedence_; ///< The precedence of this symbol.
    int line_; ///< The line that this symbol is defined on.
    int index_; ///< The index of this symbol among all symbols.
    bool nullable_; ///< True if this symbol is nullable otherwise false.
    bool referenced_in_precedence_directive_; ///< True if this symbol is referenced by a %precedence directive.
    std::set<const GrammarSymbol*, GrammarSymbolLess> first_; ///< The symbols that can start this symbol in a production or regular expression.
    std::set<const GrammarSymbol*, GrammarSymbolLess> follow_; ///< The symbols that can follow this symbol in a production or regular expression.
    std::vector<GrammarProduction*> productions_; ///< The productions that reduce to this symbol.
    std::multimap<const GrammarSymbol*, GrammarProduction*> reachable_productions_by_first_symbol_; ///< The productions reachable by right-most derivation from this symbol by their first symbol.

public:
    GrammarSymbol( const std::string& lexeme );

    const std::string& lexeme() const;
    const std::string& identifier() const;
    SymbolType symbol_type() const;
    LexemeType lexeme_type() const;
    bool literal() const;
    Associativity associativity() const;
    int precedence() const;
    int line() const;
    int index() const;
    bool nullable() const;
    bool referenced_in_precedence_directive() const;
    const std::set<const GrammarSymbol*, GrammarSymbolLess>& first() const;
    const std::set<const GrammarSymbol*, GrammarSymbolLess>& follow() const;
    const std::vector<GrammarProduction*>& productions() const;
    const std::multimap<const GrammarSymbol*, GrammarProduction*>& reachable_productions_by_first_symbol() const;
    std::multimap<const GrammarSymbol*, GrammarProduction*>::const_iterator find_reachable_productions( const GrammarSymbol& first_symbol ) const;
    GrammarSymbol* implicit_terminal() const;
    bool matches( const std::string& lexeme, SymbolType symbol_type ) const;

    void set_lexeme( const std::string& lexeme );
    void set_identifier( const std::string& identifier );
    void set_symbol_type( SymbolType symbol_type );
    void set_lexeme_type( LexemeType lexeme_type );
    void set_associativity( Associativity associativity );
    void set_precedence( int precedence );
    void set_line( int line );
    void set_index( int index );
    void set_nullable( bool nullable );
    void set_referenced_in_precedence_directive( bool referenced_in_precedence_directive );
    void append_production( GrammarProduction* production );
    void append_reachable_production( GrammarProduction* production );
    void calculate_identifier();
    void replace_by_non_terminal( const GrammarSymbol* non_terminal_symbol );    
    int add_symbol_to_first( const GrammarSymbol* symbol );
    int add_symbols_to_first( const std::set<const GrammarSymbol*, GrammarSymbolLess>& symbols );
    int add_symbol_to_follow( const GrammarSymbol* symbol );
    int add_symbols_to_follow( const std::set<const GrammarSymbol*, GrammarSymbolLess>& symbols );
    int calculate_first();
    int calculate_follow();
};

}

#endif
