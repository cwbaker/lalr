#ifndef LALR_PARSERNODE_HPP_INCLUDED
#define LALR_PARSERNODE_HPP_INCLUDED

#include "ParserUserData.hpp"
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
template <class UserData = std::shared_ptr<ParserUserData<char> >, class Char = char, class Traits = std::char_traits<Char>, class Allocator = std::allocator<Char> >
class ParserNode
{
    const ParserState* state_; ///< The state at this node.
    const ParserSymbol* symbol_; ///< The symbol at this node.
    int line_; ///< The line that generated this node or -1 if this node is unrelated to a line of source.
    std::basic_string<Char, Traits, Allocator> lexeme_; ///< The lexeme at this node (empty if this node's symbol is a non-terminal).
    UserData user_data_; ///< The user data at this node.

    public:
        ParserNode( const ParserState* state, const ParserSymbol* symbol, const UserData& user_data );
        ParserNode( const ParserState* state, const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );
        const ParserState* state() const;
        const ParserSymbol* symbol() const;
        int line() const;
        const std::basic_string<Char, Traits, Allocator>& lexeme() const;
        const UserData& user_data() const;
};

}

#endif
