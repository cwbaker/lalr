#ifndef LALR_GRAMMARPRODUCTION_HPP_INCLUDED
#define LALR_GRAMMARPRODUCTION_HPP_INCLUDED

#include <string>
#include <vector>
#include <set>

namespace lalr
{

class GrammarAction;
class GrammarSymbol;

/**
// A production specifying a symbol on the left hand side that is reduced
// from zero or more nodes on the right hand side.
*/
class GrammarProduction
{
    int index_; ///< The index of this GrammarProduction.
    GrammarSymbol* symbol_; ///< The symbol on the left hand side of this GrammarProduction.
    int line_; ///< The line in the grammar that this GrammarProduction was defined on.
    const GrammarAction* action_; ///< The action taken when this GrammarProduction is reduced or null if there is no action.
    std::vector<GrammarSymbol*> symbols_; ///< The symbols on the right hand side of this production.
    const GrammarSymbol* precedence_symbol_; /// The symbol that defines precedence for this production or null to use the right most terminal.

    public:
        GrammarProduction( int index, GrammarSymbol* symbol, int line, const GrammarAction* action );

        int index() const;
        GrammarSymbol* symbol() const;
        int line() const;
        int count_references_to_symbol( const GrammarSymbol* symbol ) const;
        const GrammarSymbol* find_rightmost_terminal_symbol() const;
        const GrammarSymbol* symbol_by_position( int position ) const;
        const std::vector<GrammarSymbol*>& symbols() const;
        int length() const;
        const GrammarAction* action() const;
        int action_index() const;
        const GrammarSymbol* precedence_symbol() const;
        int precedence() const;

        void append_symbol( GrammarSymbol* symbol );        
        void set_action( const GrammarAction* action );        
        void set_precedence_symbol( const GrammarSymbol* symbol );        
        void replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol );

        static const int INVALID_INDEX = -1;
};

}

#endif
