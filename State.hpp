#ifndef SWEET_LALR_STATE_HPP_INCLUDED
#define SWEET_LALR_STATE_HPP_INCLUDED

#include "Item.hpp"
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
    public:
        static const int INVALID_INDEX = -1;

    private:
        std::set<Item> items_; ///< The items that define the positions within the grammar that this state represents.
        std::set<Transition> transitions_; ///< The available transitions from this state.
        bool processed_; ///< True if this state has been processed during state machine generation otherwise false.
        int index_; ///< The index of this state.

    public:
        State();

        int add_item( Production* production, int position );
        int add_lookahead_symbols( Production* production, int position, const std::set<const LalrSymbol*>& lookahead_symbols );
        const std::set<Item>& get_items() const;
        void describe( std::string* description ) const;
        std::string description() const;

        void add_transition( const LalrSymbol* symbol, State* state );
        void add_transition( const LalrSymbol* symbol, const LalrSymbol* reduced_symbol, int reduced_length, int precedence, int action );
        void add_transition( const std::set<const LalrSymbol*>& symbols, const LalrSymbol* reduced_symbol, int reduced_length, int precedence, int action );
        void generate_indices_for_transitions();
        Transition* find_transition_by_symbol( const LalrSymbol* symbol );
        const Transition* find_transition_by_symbol( const LalrSymbol* symbol ) const;
        const std::set<Transition>& get_transitions() const;

        void set_processed( bool processed );
        bool is_processed() const;

        void set_index( int index );
        int get_index() const;

        bool operator<( const State& state ) const;
};

}

}

#endif
