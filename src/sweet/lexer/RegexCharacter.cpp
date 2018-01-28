//
// RegexCharacter.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include "RegexCharacter.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::lexer;

/**
// Constructor.
//
// @param begin_character
//  The first character in the represented interval.
//
// @param end_character
//  One past the last character in the represented interval.
*/
RegexCharacter::RegexCharacter( int begin_character, int end_character )
: begin_character_( begin_character ),
  end_character_( end_character )
{
    SWEET_ASSERT( begin_character_ < end_character_ );
}

/**
// Get the first character in the represented interval.
//
// @return
//  The begin character.
*/
int RegexCharacter::get_begin_character() const
{
    return begin_character_;
}


/**
// Get the character one past the last character in the represented interval.
//
// @return
//  The end character.
*/
int RegexCharacter::get_end_character() const
{
    return end_character_;
}


/**
// Less than operator.
//
// @param regex_character
//  The character to compare with.
//
// @return
//  True if the end of the interval represented by this character is less 
//  than the beginning of the interval represented by \e regex_character.
*/
bool RegexCharacter::operator<( const RegexCharacter& regex_character ) const
{
    return end_character_ < regex_character.begin_character_;
}
