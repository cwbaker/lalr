#ifndef SWEET_LALR_LEXERSTATE_HPP_INCLUDED
#define SWEET_LALR_LEXERSTATE_HPP_INCLUDED

namespace sweet
{

namespace lalr
{

class LexerTransition;

/**
// A state in a lexical analyzer's state machine.
*/
class LexerState
{
public:
    int index; ///< Index of this state.
    int length; ///< Number of transitions from this state.
    const LexerTransition* transitions; ///< Transitions from this state.
    const void* symbol; ///< The symbol that this state recognizes or null if this state doesn't recognize a symbol.

    const LexerTransition* find_transition_by_character( int character ) const;
};

}

}

#endif
