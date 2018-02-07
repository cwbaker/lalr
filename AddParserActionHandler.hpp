#ifndef SWEET_PARSER_ADDHANDLER_HPP_INCLUDED
#define SWEET_PARSER_ADDHANDLER_HPP_INCLUDED

#include <functional>

namespace sweet
{

namespace lalr
{

class LalrSymbol;
template <class Iterator, class UserData, class Char, class Traits, class Allocator> class Parser;

/**
// A helper that provides a convenient syntax for adding handlers to a %Parser.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
class AddParserActionHandler
{
    typedef std::function<UserData (const LalrSymbol* symbol, const ParserNode<UserData, Char, Traits, Allocator>* start, const ParserNode<UserData, Char, Traits, Allocator>* finish)> LalrActionFunction;

    Parser<Iterator, UserData, Char, Traits, Allocator>* parser_; ///< The Parser to add handlers to.

    public:
        AddParserActionHandler( Parser<Iterator, UserData, Char, Traits, Allocator>* parser );
        const AddParserActionHandler& default_action( LalrActionFunction function ) const;
        const AddParserActionHandler& operator()( const char* identifier, LalrActionFunction function ) const;
};

}

}

#endif
