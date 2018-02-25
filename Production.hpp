#ifndef SWEET_LALR_PRODUCTION_HPP_INCLUDED
#define SWEET_LALR_PRODUCTION_HPP_INCLUDED

#include <string>
#include <vector>
#include <set>

namespace sweet
{

namespace lalr
{

class Action;
class Symbol;

/**
// A production specifying a symbol on the left hand side that is reduced
// from zero or more nodes on the right hand side.
*/
class Production
{
    int index_; ///< The index of this Production.
    Symbol* symbol_; ///< The symbol on the left hand side of this Production.
    int line_; ///< The line in the grammar that this Production was defined on.
    const Action* action_; ///< The action taken when this Production is reduced or null if there is no action.
    std::vector<Symbol*> symbols_; ///< The symbols on the right hand side of this production.
    const Symbol* precedence_symbol_; /// The symbol that defines precedence for this production or null to use the right most terminal.

    public:
        Production( int index, Symbol* symbol, int line, const Action* action );

        int index() const;
        Symbol* symbol() const;
        int line() const;
        int count_references_to_symbol( const Symbol* symbol ) const;
        const Symbol* find_rightmost_terminal_symbol() const;
        const Symbol* symbol_by_position( int position ) const;
        const std::vector<Symbol*>& symbols() const;
        int length() const;
        std::string description() const;
        void describe( std::string* description ) const;
        const Action* action() const;
        int action_index() const;
        const Symbol* precedence_symbol() const;
        int precedence() const;

        void append_symbol( Symbol* symbol );        
        void set_action( const Action* action );        
        void set_precedence_symbol( const Symbol* symbol );        
        void replace_references_to_symbol( Symbol* to_symbol, Symbol* with_symbol );

        static const int INVALID_INDEX = -1;
};

}

}

#endif
