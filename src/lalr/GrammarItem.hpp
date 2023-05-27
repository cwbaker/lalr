#pragma once

namespace lalr
{

class GrammarProduction;

/**
// An item that defines the positions in the grammar that a state 
// represents.
*/
class GrammarItem
{
    unsigned short production_; ///< The index of the production of this item.
    unsigned short position_; ///< The position of the dot in this item.
    mutable int index_;

public:
    inline GrammarItem();
    inline GrammarItem( GrammarProduction* production, int position );
    inline int index() const;
    inline int production() const;
    inline int position() const;
    inline bool dot_at_beginning() const;
    inline bool operator<( const GrammarItem& item ) const;
    inline void set_index( int index ) const;
};

}

#include "GrammarItem.ipp"
