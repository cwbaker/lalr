#ifndef SWEET_LEXER_ADDLEXERACTIONHANDLER_IPP_INCLUDED
#define SWEET_LEXER_ADDLEXERACTIONHANDLER_IPP_INCLUDED

#include "AddLexerActionHandler.hpp"
#include "assert.hpp"

namespace sweet
{

namespace lalr
{

/**
// Constructor.
//
// @param lexer
//  The %Lexer to add actions to (assumed not null).
*/
template <class Iterator, class Char, class Traits, class Allocator>
AddLexerActionHandler<Iterator, Char, Traits, Allocator>::AddLexerActionHandler( Lexer<Iterator, Char, Traits, Allocator>* lexer )
: lexer_( lexer )
{
    SWEET_ASSERT( lexer_ );
}


/**
// Set the function to call when the lexer action \e identifier is taken.
//
// @param identifier
//  The identifier of the lexer action to assign a function to (matches the
//  identifier specified between ':' characters in a regular expression).
//
// @param function
//  The function to call when the lexer action needs to be taken.
//
// @return
//  This %AddLexerActionHandler.
*/
template <class Iterator, class Char, class Traits, class Allocator>
const AddLexerActionHandler<Iterator, Char, Traits, Allocator>& 
AddLexerActionHandler<Iterator, Char, Traits, Allocator>::operator()( const char* identifier, LexerActionFunction function ) const
{
    SWEET_ASSERT( identifier );
    SWEET_ASSERT( lexer_ );
    lexer_->set_action_handler( identifier, function );
    return *this;
}

}

}

#endif
