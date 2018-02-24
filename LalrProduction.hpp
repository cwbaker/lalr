#ifndef SWEET_LALR_LALRPRODUCTION_HPP_INCLUDED
#define SWEET_LALR_LALRPRODUCTION_HPP_INCLUDED

#include <string>
#include <vector>
#include <set>

namespace sweet
{

namespace lalr
{

class LalrAction;
class LalrSymbol;

/**
// A production specifying a symbol on the left hand side that is reduced
// from zero or more nodes on the right hand side.
*/
class LalrProduction
{
    int index_; ///< The index of this LalrProduction.
    LalrSymbol* symbol_; ///< The symbol on the left hand side of this LalrProduction.
    int line_; ///< The line in the grammar that this LalrProduction was defined on.
    const LalrAction* action_; ///< The action taken when this LalrProduction is reduced or null if there is no action.
    std::vector<LalrSymbol*> symbols_; ///< The symbols on the right hand side of this production.
    const LalrSymbol* precedence_symbol_; /// The symbol that defines precedence for this production or null to use the right most terminal.

    public:
        LalrProduction( int index, LalrSymbol* symbol, int line, const LalrAction* action );

        int index() const;
        LalrSymbol* symbol() const;
        int line() const;
        int count_references_to_symbol( const LalrSymbol* symbol ) const;
        const LalrSymbol* find_rightmost_terminal_symbol() const;
        const LalrSymbol* symbol_by_position( int position ) const;
        const std::vector<LalrSymbol*>& symbols() const;
        int length() const;
        std::string description() const;
        void describe( std::string* description ) const;
        const LalrAction* action() const;
        int action_index() const;
        const LalrSymbol* precedence_symbol() const;
        int precedence() const;

        void append_symbol( LalrSymbol* symbol );        
        void set_action( const LalrAction* action );        
        void set_precedence_symbol( const LalrSymbol* symbol );        
        void replace_references_to_symbol( LalrSymbol* to_symbol, LalrSymbol* with_symbol );

        static const int INVALID_INDEX = -1;
};

}

}

#endif
