#ifndef LALR_ASSOCIATIVITY_HPP_INCLUDED
#define LALR_ASSOCIATIVITY_HPP_INCLUDED

namespace lalr
{

/**
// The associativity of a symbol.
//
// @relates Symbol
*/
enum Associativity
{
    ASSOCIATE_NULL, ///< The symbol has undefined associativity.
    ASSOCIATE_NONE, ///< The symbol has no associativity.
    ASSOCIATE_PREC, ///< The symbol has only precedence level.
    ASSOCIATE_LEFT, ///< The symbol associates to the left.
    ASSOCIATE_RIGHT ///< The symbol associates to the right.
};

}

#endif
