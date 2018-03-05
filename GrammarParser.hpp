#ifndef SWEET_LALR_GRAMMARPARSER_HPP_INCLUDED
#define SWEET_LALR_GRAMMARPARSER_HPP_INCLUDED

#include <string>

namespace sweet
{

namespace lalr
{

class Grammar;
class ParserStateMachine;

class GrammarParser
{
    Grammar* grammar_;
    const char* position_;
    const char* end_;
    int line_;
    std::string lexeme_;
    bool successful_;

public:
    GrammarParser();
    bool parse( const char* start, const char* finish, Grammar* grammar );

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
    bool match_whitespace();
    bool match_end();
    bool match( const char* lexeme );
    bool expect( const char* lexeme );
};

}

}

#endif
