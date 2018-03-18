#ifndef SWEET_LALR_GRAMMARPRODUCTION_HPP_INCLUDED
#define SWEET_LALR_GRAMMARPRODUCTION_HPP_INCLUDED

#include <string>
#include <vector>
#include <set>

namespace sweet
{

namespace lalr
{

class GrammarAction;
class Symbol;

/**
// A production specifying a symbol on the left hand side that is reduced
// from zero or more nodes on the right hand side.
*/
class GrammarProduction
{
    int index_; ///< The index of this GrammarProduction.
    Symbol* symbol_; ///< The symbol on the left hand side of this GrammarProduction.
    int line_; ///< The line in the grammar that this GrammarProduction was defined on.
    const GrammarAction* action_; ///< The action taken when this GrammarProduction is reduced or null if there is no action.
    std::vector<Symbol*> symbols_; ///< The symbols on the right hand side of this production.
    const Symbol* precedence_symbol_; /// The symbol that defines precedence for this production or null to use the right most terminal.

    public:
        GrammarProduction( int index, Symbol* symbol, int line, const GrammarAction* action );

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
        const GrammarAction* action() const;
        int action_index() const;
        const Symbol* precedence_symbol() const;
        int precedence() const;

        void append_symbol( Symbol* symbol );        
        void set_action( const GrammarAction* action );        
        void set_precedence_symbol( const Symbol* symbol );        
        void replace_references_to_symbol( Symbol* to_symbol, Symbol* with_symbol );

        static const int INVALID_INDEX = -1;
};

}

}

#endif
