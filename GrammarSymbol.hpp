#ifndef SWEET_PARSER_GRAMMARSYMBOL_HPP_INCLUDED
#define SWEET_PARSER_GRAMMARSYMBOL_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"
#include <string>
#include <vector>

namespace sweet
{
    
namespace lalr
{

class GrammarProduction;

class GrammarSymbol
{
    std::string lexeme_;
    std::string identifier_;
    SymbolType symbol_type_;
    LexemeType lexeme_type_;
    Associativity associativity_;
    int precedence_;
    std::vector<GrammarProduction*> productions_;

public:
    GrammarSymbol( const char* lexeme );

    const std::string& lexeme() const;
    const std::string& identifier() const;
    SymbolType symbol_type() const;
    LexemeType lexeme_type() const;
    Associativity associativity() const;
    int precedence() const;
    const std::vector<GrammarProduction*>& productions() const;

    void set_lexeme( const std::string& lexeme );
    void set_identifier( const std::string& identifier );
    void set_symbol_type( SymbolType symbol_type );
    void set_lexeme_type( LexemeType lexeme_type );
    void set_associativity( Associativity associativity );
    void set_precedence( int precedence );
    void append_production( GrammarProduction* production );
};
    
}

}

#endif
