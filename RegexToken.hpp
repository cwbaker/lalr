#ifndef SWEET_LALR_LEXERTOKEN_HPP_INCLUDED
#define SWEET_LALR_LEXERTOKEN_HPP_INCLUDED

#include "RegexTokenType.hpp"
#include <string>

namespace sweet
{

namespace lalr
{

/**
// A regular expression or literal to be recognized by a lexical analyzer.
*/
class RegexToken
{
    RegexTokenType type_; ///< The type of this RegexToken (literal or regular expression).
    int line_; ///< The line to use when resolving token conflicts and reporting errors.
    const void* symbol_; ///< The symbol to return when this token is matched in input.
    std::string lexeme_; ///< The literal or regular expression pattern to match for this token.
    
    public:
        RegexToken( RegexTokenType type, int line, const void* symbol, const std::string& lexeme );
        RegexTokenType type() const;
        int line() const;
        const void* symbol() const;
        const std::string& lexeme() const;
};

}

}

#endif