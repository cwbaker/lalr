#ifndef SWEET_LALR_PARSERSTATE_HPP_INCLUDED
#define SWEET_LALR_PARSERSTATE_HPP_INCLUDED

namespace sweet
{

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

    const ParserTransition* find_transition_by_symbol( const ParserSymbol* symbol ) const;
};

}

}

#endif
