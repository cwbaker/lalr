#ifndef SWEET_PARSER_SYMBOLASSOCIATIVITY_HPP_INCLUDED
#define SWEET_PARSER_SYMBOLASSOCIATIVITY_HPP_INCLUDED

namespace sweet
{

namespace lalr
{

/**
// The associativity of a symbol.
//
// @relates ParserSymbol
*/
enum SymbolAssociativity
{
    ASSOCIATE_NULL, ///< The symbol has undefined associativity.
    ASSOCIATE_NONE, ///< The symbol has no associativity.
    ASSOCIATE_LEFT, ///< The symbol associates to the left.
    ASSOCIATE_RIGHT ///< The symbol associates to the right.
};

}

}

#endif
