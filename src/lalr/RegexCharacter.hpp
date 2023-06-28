#ifndef LALR_REGEXCHARACTER_HPP_INCLUDED
#define LALR_REGEXCHARACTER_HPP_INCLUDED

#include "assert.hpp"

namespace lalr
{

/**
// @internal
//
// A range of characters in a regular expression.
*/
class RegexCharacter
{
    int begin_character_; ///< The first character in the range of characters.
    int end_character_; ///< One past the last character in the range of characters.

public:
    RegexCharacter( int begin_character, int end_character )
        : begin_character_( begin_character )
        , end_character_( end_character )
    {
        LALR_ASSERT( begin_character_ < end_character_ );
    }
/**
// Get the first character in the represented interval.
//
// @return
//  The begin character.
*/
    int begin_character() const {return begin_character_;}
/**
// Get the character one past the last character in the represented interval.
//
// @return
//  The end character.
*/
    int end_character() const {return end_character_;}
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
    bool operator<( const RegexCharacter& regex_character ) const
    {
        return end_character_ < regex_character.begin_character_;
    }
};

}

#endif
