#ifndef LALR_REGEXCHARACTER_HPP_INCLUDED
#define LALR_REGEXCHARACTER_HPP_INCLUDED

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
        RegexCharacter( int begin_character, int end_character );        
        int begin_character() const;
        int end_character() const;
        bool operator<( const RegexCharacter& regex_character ) const;
};

}

#endif
