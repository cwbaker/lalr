#ifndef LALR_PARSERSYMBOL_HPP_INCLUDED
#define LALR_PARSERSYMBOL_HPP_INCLUDED

#include "SymbolType.hpp"

namespace lalr
{

/**
// A symbol used by the parser
*/
class ParserSymbol
{
public:
    int index; ///< The index of this symbol.
    SymbolType type; ///< The type of this symbol.
    const char* identifier; ///< The identifier of this symbol.
    const char* lexeme; ///< The lexeme of this symbol or null if this symbol is non-terminal.
    const char* label; ///< The human-readable label for this symbol.
};

}

#endif
