#ifndef LALR_LEXERTOKEN_HPP_INCLUDED
#define LALR_LEXERTOKEN_HPP_INCLUDED

#include "RegexTokenType.hpp"
#include <string>
#include <vector>

namespace lalr
{

/**
// A regular expression or literal to be recognized by a lexical analyzer.
*/
class RegexToken
{
    RegexTokenType type_; ///< The type of this RegexToken (literal or regular expression).
    int line_; ///< The line to use when resolving token conflicts and reporting errors.
    int column_; ///< The column to use when resolving token conflicts and reporting errors.
    const void* symbol_; ///< The symbol to return when this token is matched in input.
    std::string lexeme_; ///< The literal or regular expression pattern to match for this token.
    mutable std::vector<const RegexToken*> conflicted_with_; ///< The RegexTokens that this RegexToken has conflicted with.

public:
    RegexToken( RegexTokenType type, int line, int column, const void* symbol, const char* lexeme );
    RegexToken( const RegexToken& token, const void* symbol );
    RegexTokenType type() const;
    int line() const;
    int column() const;
    const void* symbol() const;
    const std::string& lexeme() const;
    bool conflicted_with( const RegexToken* token ) const;
    void add_conflicted_with( const RegexToken* token ) const;
};

}

#endif
