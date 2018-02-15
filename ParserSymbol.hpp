#ifndef SWEET_LALR_PARSERSYMBOL_HPP_INCLUDED
#define SWEET_LALR_PARSERSYMBOL_HPP_INCLUDED

#include "SymbolType.hpp"
#include "LexemeType.hpp"
#include "Associativity.hpp"

namespace sweet
{

namespace lalr
{

/**
// A symbol used by the parser
*/
class ParserSymbol
{
public:
    int index; ///< The index of this symbol.
    const char* identifier; ///< The identifier of this symbol.
    const char* lexeme; ///< The lexeme of this symbol or null if this symbol is non-terminal.

    ParserSymbol();
    ~ParserSymbol();
    void reset( int iindex, const char* iidentifier, const char* llexeme );
};

}

}

#endif
