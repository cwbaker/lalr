#ifndef LALR_GRAMMARACTION_HPP_INCLUDED
#define LALR_GRAMMARACTION_HPP_INCLUDED

#include <string>

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
    GrammarAction( int index, const std::string& identifier );
    int index() const;
    const std::string& identifier() const;
    static const int INVALID_INDEX = -1;
};

}

#endif
