#ifndef LALR_GRAMMARACTION_HPP_INCLUDED
#define LALR_GRAMMARACTION_HPP_INCLUDED

#include <string>
#include "assert.hpp"

namespace lalr
{

/**
// An action that is attached to a %parser reduction.
*/
class GrammarAction
{
    int index_; ///< The index of this action.
    std::string identifier_; ///< The identifier of this action.

public:
/**
// Constructor.
//
// @param index
//  The index of this action (assumed >= 0).
//
// @param identifier
//  The identifier of this action (assumed not empty).
*/
    GrammarAction( int index, const std::string& identifier )
    : index_( index )
    , identifier_( identifier )
    {
        LALR_ASSERT( index_ >= 0 );
        LALR_ASSERT( !identifier_.empty() );
    }
    int index() const { return index_;};
    const std::string& identifier() const { return identifier_;};
    static const int INVALID_INDEX = -1;
};

}

#endif
