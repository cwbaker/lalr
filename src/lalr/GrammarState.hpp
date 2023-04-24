#ifndef LALR_GRAMMARSTATE_HPP_INCLUDED
#define LALR_GRAMMARSTATE_HPP_INCLUDED

#include "GrammarItem.hpp"
#include "GrammarTransition.hpp"
#include "GrammarSymbolLess.hpp"
#include <vector>
#include <set>

namespace lalr
{

class GrammarSymbol;
class GrammarProduction;

/**
// A state in a parser's state machine.
*/
class GrammarState
{
    std::set<GrammarItem> items_; ///< The items that define the positions within the grammar that this state represents.
    std::vector<GrammarTransition*> transitions_by_symbol_index_; ///< Transitions from this state by symbol index.
    std::vector<GrammarTransition*> transitions_; ///< Transitions from this state.
    bool processed_; ///< True if this state has been processed during state machine generation otherwise false.
    int index_; ///< The index of this state.

public:
    GrammarState();
    GrammarState( GrammarState&& state );
    GrammarState( const GrammarState& state );

    GrammarItem* find_item( GrammarProduction* production, int position ) const;
    const std::set<GrammarItem>& items() const { return items_; }
    const GrammarTransition* find_transition_by_symbol( const GrammarSymbol* symbol ) const;
    const std::vector<GrammarTransition*>& transitions() const;
    int count_valid_transitions() const;
    std::string label() const;
    bool processed() const;
    int index() const;
    bool operator<( const GrammarState& state ) const;

    int add_item( GrammarProduction* production, int position );
    void add_shift_transition( GrammarTransition* transition );
    void add_reduce_transition( GrammarTransition* transition );
    GrammarTransition* find_transition_by_symbol( const GrammarSymbol* symbol );
    void set_processed( bool processed );
    void set_index( int index );

    static const int INVALID_INDEX = -1;

private:
    void add_transition( GrammarTransition* transition );

    GrammarState& operator=( GrammarState&& state ) = delete;
    GrammarState& operator=( const GrammarState& state ) = delete;
};

}

#endif
