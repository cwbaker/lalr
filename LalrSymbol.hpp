#ifndef SWEET_LALR_LALRSYMBOL_HPP_INCLUDED
#define SWEET_LALR_LALRSYMBOL_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include <memory>
#include <set>
#include <vector>
#include <string>

namespace sweet
{

namespace lalr
{

class LalrProduction;

/**
// A symbol parsed as part of a grammar rule.
*/
class LalrSymbol : public std::enable_shared_from_this<LalrSymbol>
{
    public:
        static const int INVALID_INDEX = -1;
        
    private:
        SymbolType type_; ///< The type of this symbol.
        std::string lexeme_; ///< The lexeme for this symbol.
        std::string identifier_; ///< The identifier for this symbol (generated from its lexeme).
        int line_; ///< The line that this symbol was defined on.
        std::vector<LalrProduction*> productions_; ///< The productions for this symbol.
        int precedence_; ///< The precedence of this symbol (0 indicates no precedence).
        Associativity associativity_; ///< The associativity of this symbol.
        LexemeType lexeme_type_; ///< The type of lexeme for this terminal symbol.
        bool nullable_; ///< True if this symbol is nullable otherwise false.
        std::set<const LalrSymbol*> first_; ///< The symbols that can start this symbol in a production or regular expression.
        std::set<const LalrSymbol*> follow_; ///< The symbols that can follow this symbol in a production or regular expression.
        int index_; ///< The index of this symbol.

    public:
        LalrSymbol();
        LalrSymbol( SymbolType type, const std::string& lexeme, int line );

        const std::string& get_lexeme() const;
        const std::string& get_identifier() const;

        void set_type( SymbolType type );
        SymbolType get_type() const;

        void set_line( int line );
        int get_line() const;

        void append_production( LalrProduction* production );
        const std::vector<LalrProduction*>& get_productions() const;

        bool is_nullable() const;
        void replace_by_non_terminal( const LalrSymbol* non_terminal_symbol );
        LalrSymbol* get_implicit_terminal() const;

        int add_symbol_to_first( const LalrSymbol* symbol );
        int add_symbols_to_first( const std::set<const LalrSymbol*>& symbols );
        const std::set<const LalrSymbol*>& get_first() const;

        int add_symbol_to_follow( const LalrSymbol* symbol );
        int add_symbols_to_follow( const std::set<const LalrSymbol*>& symbols );
        const std::set<const LalrSymbol*>& get_follow() const;
        
        void set_precedence( int precedence );
        int get_precedence() const;
        
        void set_associativity( Associativity associativity );
        Associativity get_associativity() const;
        
        void set_lexeme_type( LexemeType lexeme_type );
        LexemeType lexeme_type() const;

        void calculate_identifier();
        int calculate_first();
        int calculate_follow();

        void set_index( int index );
        int get_index() const;

        void describe( std::string* description ) const;
        static void describe( const std::set<const LalrSymbol*>& symbols, std::string* description );
        std::string description() const;

    private:
        const char* name( int character ) const;
};

}

}

#endif
