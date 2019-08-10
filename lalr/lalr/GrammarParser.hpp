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
    ErrorPolicy* error_policy_;
    Grammar* grammar_;
    const char* position_;
    const char* end_;
    int line_;
    std::string lexeme_;
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
    bool match_without_skipping_whitespace( const char* lexeme );
    bool expect( const char* lexeme );
    void error( int line, int column, int error, const char* format, ... );
    static bool is_newline( int character );
};

}

#endif
