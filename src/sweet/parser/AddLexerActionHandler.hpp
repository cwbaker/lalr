#ifndef SWEET_LEXER_ADDLEXERACTIONHANDLER_HPP_INCLUDED
#define SWEET_LEXER_ADDLEXERACTIONHANDLER_HPP_INCLUDED

#include <functional>

namespace sweet
{

namespace parser
{

template <class Iterator, class Char, class Traits, class Allocator> class Lexer;

/**
// A helper that provides a convenient syntax for adding handlers to a %Lexer.
*/
template <class Iterator, class Char, class Traits, class Allocator>
class AddLexerActionHandler
{
    typedef std::function<void (Iterator* begin, Iterator end, std::basic_string<Char, Traits, Allocator>* lexeme, const void** symbol)> LexerActionFunction;

    Lexer<Iterator, Char, Traits, Allocator>* lexer_; ///< The Lexer to add handlers to.

    public:
        AddLexerActionHandler( Lexer<Iterator, Char, Traits, Allocator>* lexer );
        const AddLexerActionHandler& operator()( const char* identifier, LexerActionFunction function ) const;
};

}

}

#endif
