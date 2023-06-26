#ifndef LALR_REGEXSTATE_HPP_INCLUDED
#define LALR_REGEXSTATE_HPP_INCLUDED

#include "RegexItem.hpp"
#include "RegexTransition.hpp"
#include <string>
#include <set>
#include "assert.hpp"

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
    const std::set<RegexItem>& get_items() const {return items_;};
    const RegexTransition* find_transition_by_character( int character ) const;
    const std::set<RegexTransition>& get_transitions() const {return transitions_;}
    const void* get_symbol() const {return symbol_;}
    bool is_processed() const {return processed_;}
    int index() const {return index_;}
    bool operator<( const RegexState& state ) const;
    int add_item( const std::set<RegexNode*, RegexNodeLess>& next_nodes );
    void add_transition( int begin, int end, RegexState* state );
/**
// Set the symbol that this state matches.
//
// @param symbol
//  The symbol to set this state as matching (assumed not null).
*/
    void set_symbol( const void* symbol )
    {
        LALR_ASSERT( !symbol_ );
        symbol_ = symbol;
    }
/**
// Set whether or not this state has been processed.
//
// @param processed
//  True to set this state as processed.
*/
    void set_processed( bool processed ) {processed_ = processed;}
    void set_index( int index ) {index_ = index;}
};

/**
// Indirectly compare objects through two `std::unique_ptr<>` objects.
*/
struct RegexStateLess
{
    bool operator()( const std::unique_ptr<RegexState>& lhs, const std::unique_ptr<RegexState>& rhs ) const
    {
        return *lhs < *rhs;
    }
};

}

#endif
