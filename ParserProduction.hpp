#ifndef SWEET_PARSER_PRODUCTION_HPP_INCLUDED
#define SWEET_PARSER_PRODUCTION_HPP_INCLUDED

#include <string>
#include <vector>
#include <set>

namespace sweet
{

namespace lalr
{

class ParserAction;
class ParserSymbol;

/**
// A production specifying a symbol on the left hand side that is reduced
// from zero or more nodes on the right hand side.
*/
class ParserProduction
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        int index_; ///< The index of this ParserProduction.
        ParserSymbol* symbol_; ///< The symbol on the left hand side of this ParserProduction.
        int line_; ///< The line in the grammar that this ParserProduction was defined on.
        const ParserAction* action_; ///< The action taken when this ParserProduction is reduced or null if there is no action.
        std::vector<ParserSymbol*> symbols_; ///< The symbols on the right hand side of this production.
        const ParserSymbol* precedence_symbol_; /// The symbol that defines precedence for this production or null to use the right most terminal.

    public:
        ParserProduction( int index, ParserSymbol* symbol, int line, const ParserAction* action );

        int get_index() const;
        ParserSymbol* get_symbol() const;
        int get_line() const;
        int count_references_to_symbol( const ParserSymbol* symbol ) const;
        const ParserSymbol* find_rightmost_terminal_symbol() const;
        int get_length() const;
        std::string description() const;
        void describe( std::string* description ) const;
        static void describe( const std::set<const ParserProduction*>& productions, std::string* description );

        void append_symbol( ParserSymbol* symbol );
        const ParserSymbol* get_symbol_by_position( int position ) const;
        const std::vector<ParserSymbol*>& get_symbols() const;
        
        void set_action( const ParserAction* action );
        const ParserAction* get_action() const;
        
        void set_precedence_symbol( const ParserSymbol* symbol );
        int get_precedence() const;
        
        void replace_references_to_symbol( ParserSymbol* to_symbol, ParserSymbol* with_symbol );
};

}

}

#endif
