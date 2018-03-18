#ifndef SWEET_LALR_STATE_HPP_INCLUDED
#define SWEET_LALR_STATE_HPP_INCLUDED

#include "GrammarItem.hpp"
#include "Transition.hpp"
#include <memory>
#include <set>

namespace sweet
{

namespace lalr
{

/**
// A state in a parser's state machine.
*/
class State
{
    std::set<GrammarItem> items_; ///< The items that define the positions within the grammar that this state represents.
    std::set<Transition> transitions_; ///< The available transitions from this state.
    bool processed_; ///< True if this state has been processed during state machine generation otherwise false.
    int index_; ///< The index of this state.

public:
    State();

    const std::set<GrammarItem>& items() const;
    const Transition* find_transition_by_symbol( const Symbol* symbol ) const;
    const std::set<Transition>& transitions() const;
    bool processed() const;
    int index() const;
    bool operator<( const State& state ) const;

    int add_item( GrammarProduction* production, int position );
    int add_lookahead_symbols( GrammarProduction* production, int position, const std::set<const Symbol*>& lookahead_symbols );
    void add_transition( const Symbol* symbol, State* state );
    void add_transition( const Symbol* symbol, const Symbol* reduced_symbol, int reduced_length, int precedence, int action );
    void add_transition( const std::set<const Symbol*>& symbols, const Symbol* reduced_symbol, int reduced_length, int precedence, int action );
    void generate_indices_for_transitions();
    Transition* find_transition_by_symbol( const Symbol* symbol );
    void set_processed( bool processed );
    void set_index( int index );

    static const int INVALID_INDEX = -1;
};

}

}

#endif
