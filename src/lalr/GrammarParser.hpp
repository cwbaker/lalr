#ifndef LALR_GRAMMARPARSER_HPP_INCLUDED
#define LALR_GRAMMARPARSER_HPP_INCLUDED

#include <string>

namespace lalr
{

class ErrorPolicy;
class Grammar;
class ParserStateMachine;

class GrammarParser
{
    ErrorPolicy* error_policy_; ///< ErrorPolicy to report errors to.
    Grammar* grammar_; ///< Grammar to build from parsing input.
    const char* position_; ///< Current input position.
    const char* end_; ///< One past the last character of input to parse.
    const char* line_position_; ///< Current line position.
    int line_; ///< Current line number.
    std::string lexeme_; ///< Currently parsed lexeme.
    int errors_; ///< The number of errors that occured during parsing and generation.

public:
    GrammarParser();
    int parse( const char* start, const char* finish, ErrorPolicy* error_policy, Grammar* grammar );

private:
    bool match_grammar();
    bool match_statements();
    bool match_statement();
    bool match_associativity_statement();
    bool match_whitespace_statement();
    bool match_case_insensitive_statement();
    bool match_error_recovery_debug_statement();
    bool match_production_statement();
    bool match_symbols();
    bool match_symbol();
    bool match_associativity();
    bool match_expressions();
    bool match_expression();
    bool match_precedence();
    bool match_action();
    bool match_error();
    bool match_literal();
    bool match_regex();
    bool match_identifier();
    bool match_whitespace_and_comments();
    bool match_whitespace();
    bool match_line_comment();
    bool match_block_comment();
    bool match_end();
    bool match( const char* lexeme );
    bool match_word( const char* lexeme );
    bool match_without_skipping_whitespace( const char* lexeme );
    bool expect( const char* lexeme );
    void error(int line, int column, int error, const char* format, ... );
    const char* new_line( const char* position );
    static bool is_new_line( const char* position );
    int get_line_column(const char* position);
    int get_line_column(const char* position, const std::string &subtract_str_size);
};

}

#endif
