#ifndef SWEET_PARSER_PRODUCTION_HPP_INCLUDED
#define SWEET_PARSER_PRODUCTION_HPP_INCLUDED

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
    public:
        static const int INVALID_INDEX = -1;

    private:
        int index_; ///< The index of this LalrProduction.
        LalrSymbol* symbol_; ///< The symbol on the left hand side of this LalrProduction.
        int line_; ///< The line in the grammar that this LalrProduction was defined on.
        const LalrAction* action_; ///< The action taken when this LalrProduction is reduced or null if there is no action.
        std::vector<LalrSymbol*> symbols_; ///< The symbols on the right hand side of this production.
        const LalrSymbol* precedence_symbol_; /// The symbol that defines precedence for this production or null to use the right most terminal.

    public:
        LalrProduction( int index, LalrSymbol* symbol, int line, const LalrAction* action );

        int get_index() const;
        LalrSymbol* get_symbol() const;
        int get_line() const;
        int count_references_to_symbol( const LalrSymbol* symbol ) const;
        const LalrSymbol* find_rightmost_terminal_symbol() const;
        int get_length() const;
        std::string description() const;
        void describe( std::string* description ) const;
        static void describe( const std::set<const LalrProduction*>& productions, std::string* description );

        void append_symbol( LalrSymbol* symbol );
        const LalrSymbol* get_symbol_by_position( int position ) const;
        const std::vector<LalrSymbol*>& get_symbols() const;
        
        void set_action( const LalrAction* action );
        const LalrAction* get_action() const;
        int action_index() const;
        
        void set_precedence_symbol( const LalrSymbol* symbol );
        int get_precedence() const;
        
        void replace_references_to_symbol( LalrSymbol* to_symbol, LalrSymbol* with_symbol );
};

}

}

#endif
