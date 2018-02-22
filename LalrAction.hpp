#ifndef SWEET_LALR_LALRACTION_HPP_INCLUDED
#define SWEET_LALR_LALRACTION_HPP_INCLUDED

#include <string>

namespace sweet
{

namespace lalr
{

/**
// An action that is attached to a %parser reduction.
*/
class LalrAction
{
    int index_; ///< The index of this action.
    std::string identifier_; ///< The identifier of this action.

public:
    LalrAction( int index, const std::string& identifier );
    int index() const;
    const std::string& identifier() const;
    static const int INVALID_INDEX = -1;
};

}

}

#endif
