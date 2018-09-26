#ifndef LALR_PARSERUSERDATA_IPP_INCLUDED
#define LALR_PARSERUSERDATA_IPP_INCLUDED

#include "ParserUserData.hpp"
#include "assert.hpp"
#include <algorithm>

namespace lalr
{

/**
// Constructor.
//
// @param symbol
//  The symbol at this user data (assumed not null).
//
// @param lexeme
//  The lexeme at this user data.
*/
template <class Char, class Traits, class Allocator>
ParserUserData<Char, Traits, Allocator>::ParserUserData( const ParserSymbol* symbol, const std::basic_string<Char, Traits, Allocator>& lexeme )
: symbol_( symbol ),
  lexeme_( lexeme ),
  user_datas_()
{
    LALR_ASSERT( symbol_ );
}

/**
// Constructor.
//
// @param symbol
//  The symbol at this user data (assumed not null).
//
// @param user_datas
//  The number of user datas that will be added as children of this user data.
*/
template <class Char, class Traits, class Allocator>
ParserUserData<Char, Traits, Allocator>::ParserUserData( const ParserSymbol* symbol, size_t user_datas )
: symbol_( symbol ),
  lexeme_(),
  user_datas_()
{
    LALR_ASSERT( symbol_ );
    user_datas_.reserve( user_datas );
}

/**
// Constructor.
//
// @param symbol
//  The symbol at this user data.
//
// @param start
//  The first node to get user data from to add as a child of this user data.
//
// @param finish
//  One past the last node to get user data from to add as a child of this 
//  user data.
*/
template <class Char, class Traits, class Allocator>
ParserUserData<Char, Traits, Allocator>::ParserUserData( const ParserSymbol* symbol, const ParserNode* start, const ParserNode* finish )
: symbol_( symbol ),
  lexeme_(),
  user_datas_()
{
    LALR_ASSERT( start );
    LALR_ASSERT( finish );
    LALR_ASSERT( start <= finish );
    LALR_ASSERT( symbol_ );
    
    for ( const ParserNode* node = start; node != finish; ++node )
    {
        if ( node->user_data() )
        {
            user_datas_.push_back( node->user_data() );
        }
        else
        {
            std::shared_ptr<ParserUserData<char> > user_data( new ParserUserData(node->symbol(), node->lexeme()) );
            user_datas_.push_back( user_data );
        }
    }
}

/**
// Get the symbol at this user data.
//
// @return
//  The symbol.
*/
template <class Char, class Traits, class Allocator>
const ParserSymbol* ParserUserData<Char, Traits, Allocator>::symbol() const
{
    return symbol_;
}

/**
// Get the lexeme at this user data.
//
// @return
//  The lexeme.
*/
template <class Char, class Traits, class Allocator>
const std::basic_string<Char, Traits, Allocator>& ParserUserData<Char, Traits, Allocator>::lexeme() const
{
    return lexeme_;
}

/**
// Append user data as a child of this user data.
//
// @param user_data
//  The user data to append to this user data.
*/
template <class Char, class Traits, class Allocator>
void ParserUserData<Char, Traits, Allocator>::append_user_data( std::shared_ptr<ParserUserData> user_data )
{
    LALR_ASSERT( std::find(user_datas_.begin(), user_datas_.end(), user_data) == user_datas_.end() );
    user_datas_.push_back( user_data );
}

/**
// Get the user datas that are part of this user data.
//
// @return
//  The user datas.
*/
template <class Char, class Traits, class Allocator>
const std::vector<std::shared_ptr<ParserUserData<Char, Traits, Allocator> > >& ParserUserData<Char, Traits, Allocator>::user_datas() const
{
    return user_datas_;
}

}

#endif
