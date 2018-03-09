#ifndef SWEET_LALR_REGEXTRANSITION_HPP_INCLUDED
#define SWEET_LALR_REGEXTRANSITION_HPP_INCLUDED

#include <string>

namespace sweet
{

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
        int get_begin() const;
        int get_end() const;
        bool is_on_character( int character ) const;
        const RegexState* get_state() const;
        const RegexAction* get_action() const;
        void describe( std::string* description ) const;
        bool operator<( const RegexTransition& transition ) const;
};

}

}

#endif