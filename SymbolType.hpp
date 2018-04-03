#ifndef LALR_SYMBOLTYPE_HPP_INCLUDED
#define LALR_SYMBOLTYPE_HPP_INCLUDED

namespace lalr
{
    
/**
// The type of a symbol.
//
// @relates Symbol
*/
enum SymbolType
{
    SYMBOL_NULL, ///< Null type.
    SYMBOL_TERMINAL, ///< Terminal symbols.
    SYMBOL_NON_TERMINAL, ///< Non-terminal symbols.
    SYMBOL_END, ///< The end symbol.
    SYMBOL_TYPE_COUNT ///< The number of symbol types.
};

}

#endif
