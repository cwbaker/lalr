#ifndef SWEET_LALR_PARSERACTION_HPP_INCLUDED
#define SWEET_LALR_PARSERACTION_HPP_INCLUDED

#include <string>

namespace sweet
{

namespace lalr
{

/**
// An action that is attached to a %parser reduction.
*/
class ParserAction
{
public:
    static const int INVALID_INDEX = -1;
    int index; ///< The index of this action.
    const char* identifier; ///< The identifier of this action.
};

}

}

#endif
