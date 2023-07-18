#ifndef LALR_ADDPARSERACTIONHANDLER_HPP_INCLUDED
#define LALR_ADDPARSERACTIONHANDLER_HPP_INCLUDED

#include "ParserNode.hpp"
#include "ParserTransition.hpp"
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
    typedef std::function<bool (UserData &result, const UserData* data, const ParserNode<Char, Traits, Allocator>* nodes, size_t length, const ParserTransition *transition)> ParserActionFunction;
    Parser<Iterator, UserData, Char, Traits, Allocator>* parser_; ///< The Parser to add handlers to.

public:
    AddParserActionHandler( Parser<Iterator, UserData, Char, Traits, Allocator>* parser );
    const AddParserActionHandler& default_action( ParserActionFunction function ) const;
    const AddParserActionHandler& operator()( const char* identifier, ParserActionFunction function ) const;
};

}

#endif
