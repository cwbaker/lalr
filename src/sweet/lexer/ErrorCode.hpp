#ifndef SWEET_LEXER_ERRORCODE_HPP_INCLUDED
#define SWEET_LEXER_ERRORCODE_HPP_INCLUDED

namespace sweet
{

namespace lexer
{

/**
// Error codes reported by the %lexer library.
*/
enum ErrorCode
{
    LEXER_ERROR_NONE, ///< No %error.
    LEXER_ERROR_SYNTAX, ///< Syntax %error occured while parsing some input.
    LEXER_ERROR_SYMBOL_CONFLICT, ///< A lexer state matches more than one symbol.
    LEXER_ERROR_LEXICAL_ERROR ///< A lexical error occured while scanning an input sequence.
};

}

}

#endif
