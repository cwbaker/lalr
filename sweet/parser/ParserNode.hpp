//
// ParserNode.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_PARSER_PARSERNODE_HPP_INCLUDED
#define SWEET_PARSER_PARSERNODE_HPP_INCLUDED

#include "ParserUserData.hpp"
#include <sweet/pointer/ptr.hpp>
#include <string>
#include <set>

namespace sweet
{

namespace parser
{

class ParserSymbol;
class ParserProduction;
class ParserState;

/**
// An element in the parser's stack when parsing.
*/
template <class UserData = ptr<ParserUserData<char> >, class Char = char, class Traits = std::char_traits<Char>, class Allocator = std::allocator<Char> >
class ParserNode
{
    const ParserState* state_; ///< The state at this node.
    const ParserSymbol* symbol_; ///< The symbol at this node.
    std::basic_string<Char, Traits, Allocator> lexeme_; ///< The lexeme at this node (empty if this node's symbol is a non-terminal).
    UserData user_data_; ///< The user data at this node.

    public:
        ParserNode( const ParserState* state, const ParserSymbol* symbol, const UserData& user_data );
        ParserNode( const ParserState* state, const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme );
        
        const ParserState* get_state() const;        
        const ParserSymbol* get_symbol() const;
        const std::basic_string<Char, Traits, Allocator>& get_lexeme() const;
        const UserData& get_user_data() const;
};

}

}

#endif
