#ifndef SWEET_LEXER_REGEXCHARACTER_HPP_INCLUDED
#define SWEET_LEXER_REGEXCHARACTER_HPP_INCLUDED

namespace sweet
{

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
        int get_begin_character() const;
        int get_end_character() const;
        bool operator<( const RegexCharacter& regex_character ) const;
};

}

}

#endif