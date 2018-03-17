#ifndef SWEET_LALR_LEXERACTION_HPP_INCLUDED
#define SWEET_LALR_LEXERACTION_HPP_INCLUDED

namespace sweet
{

namespace lalr
{

/**
// An action that is attached to a lexical analyzer.
*/
class LexerAction
{
public:
    static const int INVALID_INDEX = -1;
    int index; ///< The index of this action.
    const char* identifier; ///< The identifier of this action.
};

}

}

#endif
