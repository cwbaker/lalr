#ifndef SWEET_LALR_SYMBOLTYPE_HPP_INCLUDED
#define SWEET_LALR_SYMBOLTYPE_HPP_INCLUDED

namespace sweet
{

namespace lalr
{
    
/**
// The type of a symbol.
//
// @relates ParserSymbol
*/
enum SymbolType
{
    SYMBOL_NULL, ///< Null type.
    SYMBOL_TERMINAL, ///< Terminal symbols.
    SYMBOL_NON_TERMINAL, ///< Non-terminal symbols.
    SYMBOL_END ///< The end symbol.
};

}

}

#endif
