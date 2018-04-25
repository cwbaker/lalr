#ifndef LALR_ADDPARSERACTIONHANDLER_HPP_INCLUDED
#define LALR_ADDPARSERACTIONHANDLER_HPP_INCLUDED

#include "ParserNode.hpp"
#include <functional>

namespace lalr
{

class ParserSymbol;
template <class Iterator, class UserData, class Char, class Traits, class Allocator> class Parser;

/**
// A helper that provides a convenient syntax for adding handlers to a %Parser.
*/
template <class Iterator, class UserData, class Char, class Traits, class Allocator>
class AddParserActionHandler
{
    typedef std::function<UserData (const lalr::ParserNode<UserData, Char, Traits, Allocator>* start, const lalr::ParserNode<UserData, Char, Traits, Allocator>* finish)> ParserActionFunction;

    Parser<Iterator, UserData, Char, Traits, Allocator>* parser_; ///< The Parser to add handlers to.

    public:
        AddParserActionHandler( Parser<Iterator, UserData, Char, Traits, Allocator>* parser );
        const AddParserActionHandler& default_action( ParserActionFunction function ) const;
        const AddParserActionHandler& operator()( const char* identifier, ParserActionFunction function ) const;
};

}

#endif
