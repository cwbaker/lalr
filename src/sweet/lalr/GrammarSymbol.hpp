#ifndef SWEET_PARSER_GRAMMARSYMBOL_HPP_INCLUDED
#define SWEET_PARSER_GRAMMARSYMBOL_HPP_INCLUDED

#include "GrammarSymbolType.hpp"
#include "SymbolAssociativity.hpp"
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
    GrammarSymbolType type_;
    SymbolAssociativity associativity_;
    int precedence_;
    std::vector<GrammarProduction*> productions_;

public:
    GrammarSymbol( const char* lexeme );

    const std::string& lexeme() const;
    const std::string& identifier() const;
    GrammarSymbolType type() const;
    SymbolAssociativity associativity() const;
    int precedence() const;
    const std::vector<GrammarProduction*>& productions() const;

    void set_lexeme( const std::string& lexeme );
    void set_identifier( const std::string& identifier );
    void set_type( GrammarSymbolType type );
    void set_associativity( SymbolAssociativity associativity );
    void set_precedence( int precedence );
    void append_production( GrammarProduction* production );
};
    
}

}

#endif
