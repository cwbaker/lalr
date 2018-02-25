#ifndef SWEET_LALR_LALRSTATE_HPP_INCLUDED
#define SWEET_LALR_LALRSTATE_HPP_INCLUDED

#include "Item.hpp"
#include "LalrTransition.hpp"
#include <memory>
#include <set>

namespace sweet
{

namespace lalr
{

/**
// A state in a parser's state machine.
*/
class LalrState
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        std::set<Item> items_; ///< The items that define the positions within the grammar that this state represents.
        std::set<LalrTransition> transitions_; ///< The available transitions from this state.
        bool processed_; ///< True if this state has been processed during state machine generation otherwise false.
        int index_; ///< The index of this state.

    public:
        LalrState();

        int add_item( LalrProduction* production, int position );
        int add_lookahead_symbols( LalrProduction* production, int position, const std::set<const LalrSymbol*>& lookahead_symbols );
        const std::set<Item>& get_items() const;
        void describe( std::string* description ) const;
        std::string description() const;

        void add_transition( const LalrSymbol* symbol, LalrState* state );
        void add_transition( const LalrSymbol* symbol, const LalrSymbol* reduced_symbol, int reduced_length, int precedence, int action );
        void add_transition( const std::set<const LalrSymbol*>& symbols, const LalrSymbol* reduced_symbol, int reduced_length, int precedence, int action );
        void generate_indices_for_transitions();
        LalrTransition* find_transition_by_symbol( const LalrSymbol* symbol );
        const LalrTransition* find_transition_by_symbol( const LalrSymbol* symbol ) const;
        const std::set<LalrTransition>& get_transitions() const;

        void set_processed( bool processed );
        bool is_processed() const;

        void set_index( int index );
        int get_index() const;

        bool operator<( const LalrState& state ) const;
};

}

}

#endif
