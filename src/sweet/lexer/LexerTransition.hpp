//
// LexerTransition.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LEXER_LEXERTRANSITION_HPP_INCLUDED
#define SWEET_LEXER_LEXERTRANSITION_HPP_INCLUDED

#include "declspec.hpp"
#include <string>

namespace sweet
{

namespace lexer
{

class LexerAction;
class LexerState;

/**
// A transition in a lexical analyzer's state machine.
*/
class SWEET_LEXER_DECLSPEC LexerTransition
{
    int begin_; ///< The first character that the transition can be made on.
    int end_; ///< One past the last character that the transition can be made on.
    const LexerState*  state_; ///< The state that is transitioned to.
    const LexerAction* action_; ///< The action that is taken on the transition or null if no action is taken.

    public:
        LexerTransition( int begin, int end, const LexerState* state, const LexerAction* action );
        int get_begin() const;
        int get_end() const;
        bool is_on_character( int character ) const;
        const LexerState* get_state() const;
        const LexerAction* get_action() const;
        void describe( std::string* description ) const;
        bool operator<( const LexerTransition& transition ) const;
};

}

}

#endif