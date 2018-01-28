#ifndef SWEET_LEXER_LEXERSTATE_HPP_INCLUDED
#define SWEET_LEXER_LEXERSTATE_HPP_INCLUDED

#include "LexerItem.hpp"
#include "LexerTransition.hpp"
#include <string>
#include <set>

namespace sweet
{

namespace lexer
{

class RegexNode;

/**
// A state in a lexical analyzer's state machine.
*/
class LexerState
{
    std::set<LexerItem>       items_;       ///< The items that define the positions within the regular expressions that this state represents.
    std::set<LexerTransition> transitions_; ///< The available transitions from this state to other states.
    const void*               symbol_;      ///< The symbol that this state recognizes or null if this state doesn't recognize a symbol.
    bool                      processed_;   ///< True if this state has been processed during state machine generation otherwise false.
    int                       index_;       ///< The index of this state.

    public:
        LexerState();

        int add_item( const std::set<RegexNode*, RegexNodeLess>& next_nodes );
        const std::set<LexerItem>& get_items() const;

        void add_transition( int begin, int end, LexerState* state );
        const LexerTransition* find_transition_by_character( int character ) const;
        const std::set<LexerTransition>& get_transitions() const;

        void set_symbol( const void* symbol );
        const void* get_symbol() const;

        void set_processed( bool processed );
        bool is_processed() const;

        void set_index( int index );
        int get_index() const;
        
        void describe( std::string* description ) const;
        bool operator<( const LexerState& state ) const;
};

}

}

#endif
