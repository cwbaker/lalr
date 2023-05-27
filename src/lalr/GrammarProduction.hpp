#pragma once

#include "GrammarSymbol.hpp"
#include "GrammarAction.hpp"
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
    int column_; ///< The column in the grammar that this GrammarProduction was defined on.
    const GrammarAction* action_; ///< The action taken when this GrammarProduction is reduced or null if there is no action.
    std::vector<GrammarSymbol*> symbols_; ///< The symbols on the right hand side of this production.
    const GrammarSymbol* precedence_symbol_; /// The symbol that defines precedence for this production or null to use the right most terminal.

public:
    inline GrammarProduction( int index, GrammarSymbol* symbol, int line, int column, const GrammarAction* action );

    inline int index() const;
    inline GrammarSymbol* symbol() const;
    inline int line() const;
    inline int column() const;
    inline int count_references_to_symbol( const GrammarSymbol* symbol ) const;
    inline bool nullable_after( int position ) const;
    inline const GrammarSymbol* find_rightmost_terminal_symbol() const;
    inline const GrammarSymbol* symbol_by_position( int position ) const;
    inline const std::vector<GrammarSymbol*>& symbols() const;
    inline int length() const;
    inline const GrammarAction* action() const;
    inline int action_index() const;
    inline const GrammarSymbol* precedence_symbol() const;
    inline int precedence() const;

    inline void append_symbol( GrammarSymbol* symbol );
    inline void set_action( const GrammarAction* action );
    inline void set_precedence_symbol( const GrammarSymbol* symbol );
    inline void replace_references_to_symbol( GrammarSymbol* to_symbol, GrammarSymbol* with_symbol );

    static const int INVALID_INDEX = -1;
};

}

#include "GrammarProduction.ipp"
