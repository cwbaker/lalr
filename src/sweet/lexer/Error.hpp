#ifndef SWEET_LEXER_ERROR_HPP_INCLUDED
#define SWEET_LEXER_ERROR_HPP_INCLUDED

#include <sweet/error/Error.hpp>
#include <sweet/error/ErrorTemplate.hpp>

namespace sweet
{

namespace lexer
{

/**
// Unique identifiers for the errors thrown from the %parser library.
//
// @relates Error
*/
enum ErrorCode
{
    LEXER_ERROR_NONE,            ///< No %error.
    LEXER_ERROR_SYNTAX,          ///< Syntax %error occured while parsing some input.
    LEXER_ERROR_SYMBOL_CONFLICT, ///< A lexer state matches more than one symbol.
    LEXER_ERROR_LEXICAL_ERROR    ///< A lexical error occured while scanning an input sequence.
};

/**
// Errors thrown from the %lexer library.
*/
class Error : public error::Error
{
    public:
        Error( int error );
};

/**
// Syntax %error.
//
// @relates Error
*/
typedef error::ErrorTemplate<LEXER_ERROR_SYNTAX, Error> SyntaxError;

/**
// A lexer state matches more than one symbol.
//
// @relates Error
*/
typedef error::ErrorTemplate<LEXER_ERROR_SYMBOL_CONFLICT, Error> LexerSymbolConflictError;

/**
// A lexical error occured while scanning an input sequence.
//
// @relates Error
*/
typedef error::ErrorTemplate<LEXER_ERROR_LEXICAL_ERROR, Error> LexerLexicalError;

}

}

#endif
