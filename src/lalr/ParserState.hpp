#ifndef LALR_PARSERSTATE_HPP_INCLUDED
#define LALR_PARSERSTATE_HPP_INCLUDED

namespace lalr
{

class ParserSymbol;
class ParserTransition;

/**
// A state in a parser's state machine.
*/
class ParserState
{
public:
    int index; ///< The index of this state.
    int length; ///< The number of transitions in this state.
    const ParserTransition* transitions; ///< The available transitions from this state.
    const char* label; ///< The state's items as a text label for debugging
};

}

#endif
