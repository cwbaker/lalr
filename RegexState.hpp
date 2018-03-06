#ifndef SWEET_LALR_REGEXSTATE_HPP_INCLUDED
#define SWEET_LALR_REGEXSTATE_HPP_INCLUDED

#include "RegexItem.hpp"
#include "RegexTransition.hpp"
#include <string>
#include <set>

namespace sweet
{

namespace lalr
{

class RegexNode;

/**
// A state in a lexical analyzer's state machine.
*/
class RegexState
{
    std::set<RegexItem> items_; ///< The items that define the positions within the regular expressions that this state represents.
    std::set<RegexTransition> transitions_; ///< The available transitions from this state to other states.
    const void* symbol_; ///< The symbol that this state recognizes or null if this state doesn't recognize a symbol.
    bool processed_; ///< True if this state has been processed during state machine generation otherwise false.
    int index_; ///< The index of this state.

    public:
        RegexState();
        const std::set<RegexItem>& get_items() const;
        const RegexTransition* find_transition_by_character( int character ) const;
        const std::set<RegexTransition>& get_transitions() const;
        const void* get_symbol() const;
        bool is_processed() const;
        int get_index() const;
        void describe( std::string* description ) const;
        bool operator<( const RegexState& state ) const;
        int add_item( const std::set<RegexNode*, RegexNodeLess>& next_nodes );
        void add_transition( int begin, int end, RegexState* state );
        void set_symbol( const void* symbol );
        void set_processed( bool processed );
        void set_index( int index );
};

}

}

#endif
