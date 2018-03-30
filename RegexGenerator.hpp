#ifndef SWEET_LALR_LEXERGENERATOR_HPP_INCLUDED
#define SWEET_LALR_LEXERGENERATOR_HPP_INCLUDED

#include "LexerToken.hpp"
#include "unique_ptr_less.hpp"
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

class ErrorPolicy;
class RegexState;
class RegexAction;
class RegexSyntaxTree;
class RegexCompiler;

/**
// @internal 
//
// Generate a lexical analyzer.
*/
class RegexGenerator
{
    ErrorPolicy* error_policy_; ///< The error policy to report errors and debug information to or null to ignore errors and debug information.
    std::vector<std::unique_ptr<RegexAction>> actions_; ///< The lexical analyzer actions.
    std::set<std::unique_ptr<RegexState>, unique_ptr_less<RegexState>> states_; ///< The states generated for the lexical analyzer.
    const RegexState* start_state_; ///< The starting state for the lexical analyzer.
    std::vector<std::pair<int, bool>> ranges_; ///< Ranges generated for the current transition while generating.

    public:
        RegexGenerator();
        ~RegexGenerator();
        void fire_error( int line, int error, const char* format, ... ) const;
        void fire_printf( const char* format, ... ) const;
        const RegexAction* add_lexer_action( const std::string& identifier );
        void generate( const std::string& regular_expression, void* symbol, RegexCompiler* allocations, ErrorPolicy* error_policy = nullptr );
        void generate( const std::vector<LexerToken>& tokens, RegexCompiler* allocations, ErrorPolicy* error_policy = nullptr );

    private:
        std::unique_ptr<RegexState> goto_( const RegexState* state, int begin, int end );
        void generate_states( const RegexSyntaxTree& syntax_tree, std::set<std::unique_ptr<RegexState>, unique_ptr_less<RegexState>>* states, const RegexState** start_state );
        void generate_indices_for_states();
        void generate_symbol_for_state( RegexState* state ) const;
        void populate_allocations( RegexCompiler* allocations ) const;
        void clear();
        void insert( int begin, int end );
};

}

}

#endif
