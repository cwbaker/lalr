#ifndef SWEET_LALR_GRAMMARSTATE_HPP_INCLUDED
#define SWEET_LALR_GRAMMARSTATE_HPP_INCLUDED

#include "GrammarItem.hpp"
#include "GrammarTransition.hpp"
#include <memory>
#include <set>

namespace sweet
{

namespace lalr
{

class GrammarSymbol;

/**
// A state in a parser's state machine.
*/
class GrammarState
{
    std::set<GrammarItem> items_; ///< The items that define the positions within the grammar that this state represents.
    std::set<GrammarTransition> transitions_; ///< The available transitions from this state.
    bool processed_; ///< True if this state has been processed during state machine generation otherwise false.
    int index_; ///< The index of this state.

public:
    GrammarState();

    const std::set<GrammarItem>& items() const;
    const GrammarTransition* find_transition_by_symbol( const GrammarSymbol* symbol ) const;
    const std::set<GrammarTransition>& transitions() const;
    bool processed() const;
    int index() const;
    bool operator<( const GrammarState& state ) const;

    int add_item( GrammarProduction* production, int position );
    int add_lookahead_symbols( GrammarProduction* production, int position, const std::set<const GrammarSymbol*>& lookahead_symbols );
    void add_transition( const GrammarSymbol* symbol, GrammarState* state );
    void add_transition( const GrammarSymbol* symbol, const GrammarSymbol* reduced_symbol, int reduced_length, int precedence, int action );
    void add_transition( const std::set<const GrammarSymbol*>& symbols, const GrammarSymbol* reduced_symbol, int reduced_length, int precedence, int action );
    void generate_indices_for_transitions();
    GrammarTransition* find_transition_by_symbol( const GrammarSymbol* symbol );
    void set_processed( bool processed );
    void set_index( int index );

    static const int INVALID_INDEX = -1;
};

}

}

#endif
