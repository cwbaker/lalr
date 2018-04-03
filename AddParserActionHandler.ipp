#ifndef LALR_ADDHANDLER_IPP_INCLUDED
#define LALR_ADDHANDLER_IPP_INCLUDED

#include "AddParserActionHandler.hpp"
#include "assert.hpp"

namespace lalr
{

/**
// Constructor.
//
// @param parser
//  The %Parser to add actions to (assumed not null).
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>::AddParserActionHandler( Parser<Iterator, UserData, Char, Traits, Allocator>* parser )
: parser_( parser )
{
    SWEET_ASSERT( parser_ );
}

/**
// Set the function to call for the default action (taken on a reduction when 
// no action is specified in the grammar).
//
// @param function
//  The function to call for the default action.
//
// @return
//  This %AddParserActionHandler.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>& 
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>::default_action( ParserActionFunction function ) const
{
    SWEET_ASSERT( parser_ );
    parser_->set_default_action_handler( function );
    return *this;
}

/**
// Set the function to call when the %parser action \e identifier is taken.
//
// @param identifier
//  The identifier of the %parser action to assign a function to (matches the
//  identifier specified between '[' and ']' characters at the end of a
//  production).
//
// @param function
//  The function to call when the %parser action needs to be taken.
//
// @return
//  This AddParserActionHandler.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
const AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>& 
AddParserActionHandler<Iterator, UserData, Char, Traits, Allocator>::operator()( const char* identifier, ParserActionFunction function ) const
{
    SWEET_ASSERT( identifier );
    SWEET_ASSERT( parser_ );
    parser_->set_action_handler( identifier, function );
    return *this;
}

}

#endif
