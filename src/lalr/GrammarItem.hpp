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
    GrammarItem();
    GrammarItem( GrammarProduction* production, int position );
    int index() const;
    int production() const;
    int position() const;
    bool dot_at_beginning() const;
    bool operator<( const GrammarItem& item ) const;
    void set_index( int index ) const;
};

}
