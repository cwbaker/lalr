#ifndef LALR_PARSERNODE_HPP_INCLUDED
#define LALR_PARSERNODE_HPP_INCLUDED

#include <memory>
#include <string>
#include <set>

namespace lalr
{

class ParserSymbol;
class ParserState;

/**
// An element in the parser's stack when parsing.
*/
template <class Char = char, class Traits = std::char_traits<Char>, class Allocator = std::allocator<Char>>
class ParserNode
{
    const ParserState* state_; ///< The state at this node.
    const ParserSymbol* symbol_; ///< The symbol at this node.
    std::basic_string<Char, Traits, Allocator> lexeme_; ///< The lexeme at this node (empty if this node's symbol is non-terminal).
    int line_; ///< The line number at the start of the lexeme at this node.

    public:
        ParserNode( const ParserState* state, const ParserSymbol* symbol, int line );
        ParserNode( const ParserState* state, const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme, int line );
        const ParserState* state() const;
        const ParserSymbol* symbol() const;
        const std::basic_string<Char, Traits, Allocator>& lexeme() const;
        int line() const;
};

}

#endif
