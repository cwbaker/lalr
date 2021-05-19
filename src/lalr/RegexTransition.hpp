#ifndef LALR_REGEXTRANSITION_HPP_INCLUDED
#define LALR_REGEXTRANSITION_HPP_INCLUDED

namespace lalr
{

class RegexAction;
class RegexState;

/**
// A transition in a lexical analyzer's state machine.
*/
class RegexTransition
{
    int begin_; ///< The first character that the transition can be made on.
    int end_; ///< One past the last character that the transition can be made on.
    const RegexState* state_; ///< The state that is transitioned to.
    const RegexAction* action_; ///< The action that is taken on the transition or null if no action is taken.

public:
    RegexTransition( int begin, int end, const RegexState* state, const RegexAction* action );
    int begin() const;
    int end() const;
    bool on_character( int character ) const;
    const RegexState* state() const;
    const RegexAction* action() const;
    bool operator<( const RegexTransition& transition ) const;
};

}

#endif
