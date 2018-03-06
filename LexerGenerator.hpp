#ifndef SWEET_LEXER_LEXERGENERATOR_HPP_INCLUDED
#define SWEET_LEXER_LEXERGENERATOR_HPP_INCLUDED

#include "LexerToken.hpp"
#include "shared_ptr_less.hpp"
#include <memory>
#include <vector>
#include <set>

namespace sweet
{

namespace error
{

class Error;

}

namespace lalr
{

class RegexState;
class RegexAction;
class LexerErrorPolicy;
class RegexSyntaxTree;

/**
// @internal 
//
// Generate a lexical analyzer.
*/
class LexerGenerator
{
    LexerErrorPolicy* event_sink_; ///< The event sink to report errors and debug information to or null to ignore errors and debug information.
    std::vector<std::shared_ptr<RegexAction>> actions_; ///< The lexical analyzer actions.
    std::set<std::shared_ptr<RegexState>, shared_ptr_less<RegexState>> states_; ///< The states generated for the lexical analyzer.
    std::set<std::shared_ptr<RegexState>, shared_ptr_less<RegexState>> whitespace_states_; ///< The states generated for the whitespace lexical analyzer.
    const RegexState* start_state_; ///< The starting state for the lexical analyzer.
    const RegexState* whitespace_start_state_; ///< The starting state for the whitespace lexical analyzer.
    std::vector<std::pair<int, bool>> ranges_; ///< Ranges generated for the current transition while generating.

    public:
        LexerGenerator( const LexerToken& token, LexerErrorPolicy* event_sink );
        LexerGenerator( const std::vector<LexerToken>& tokens, const std::vector<LexerToken>& whitespace_tokens, LexerErrorPolicy* event_sink );

        std::vector<std::shared_ptr<RegexAction> >& actions();
        std::set<std::shared_ptr<RegexState>, shared_ptr_less<RegexState>>& states();
        std::set<std::shared_ptr<RegexState>, shared_ptr_less<RegexState>>& whitespace_states();
        const RegexState* start_state() const;
        const RegexState* whitespace_start_state() const;
        const RegexAction* add_lexer_action( const std::string& identifier );

        void fire_error( int line, int error, const char* format, ... ) const;
        void fire_printf( const char* format, ... ) const;

    private:
        std::shared_ptr<RegexState> goto_( const RegexState* state, int begin, int end );
        void generate_states( const RegexSyntaxTree& syntax_tree, std::set<std::shared_ptr<RegexState>, shared_ptr_less<RegexState>>* states, const RegexState** start_state );
        void generate_indices_for_states();
        void generate_symbol_for_state( RegexState* state ) const;

        void clear();
        void insert( int begin, int end );
};

}

}

#endif