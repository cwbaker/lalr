
#include <stdio.h>
#include <stdarg.h>
#include <sweet/lexer/LexerStateMachine.hpp>
#include <sweet/lexer/Lexer.ipp>
#include <sweet/lexer/LexerToken.hpp>
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/ParserGrammar.hpp>
#include <sweet/parser/Parser.ipp>

using namespace std;
using namespace sweet;
using namespace sweet::lexer;
using namespace sweet::parser;

static int add( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() + start[2].get_user_data();
}

static int subtract( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() - start[2].get_user_data();
}

static int multiply( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() * start[2].get_user_data();
}

static int divide( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[0].get_user_data() / start[2].get_user_data();
}

static int compound( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return start[1].get_user_data();
}

static int integer( const ParserSymbol* symbol, const ParserNode<int>* start, const ParserNode<int>* finish )
{
    return ::atoi( start[0].get_lexeme().c_str() );
}

void parser_calculator_with_grammar_example()
{
// calculator {
//    %left '+' '-';
//    %left '*' '/';
//    %none integer;
//
//    expr: expr '+' expr [add]
//        | expr '-' expr [subtract]
//        | expr '*' expr [multiply]
//        | expr '/' expr [divide]
//        | '(' expr ')' [compound]
//        | integer [integer]
//    ;
//
//    integer: "[0-9]+";
//    whitespace: "[ \t\r\n]*"
// }"

    vector<LexerToken> tokens;
    vector<LexerToken> whitespace_tokens;
    ParserGrammar grammar;

    {
        ParserSymbol* plus = grammar.add_symbol( SYMBOL_TERMINAL, "+", 2 );
        plus->set_associativity( ASSOCIATE_LEFT );
        plus->set_precedence( 1 );
        tokens.push_back( LexerToken(TOKEN_LITERAL, 2, plus, "+") );

        ParserSymbol* minus = grammar.add_symbol( SYMBOL_TERMINAL, "-", 2 );
        minus->set_associativity( ASSOCIATE_LEFT );
        minus->set_precedence( 1 );
        tokens.push_back( LexerToken(TOKEN_LITERAL, 2, minus, "-") );

        ParserSymbol* multiply = grammar.add_symbol( SYMBOL_TERMINAL, "*", 3 );
        multiply->set_associativity( ASSOCIATE_LEFT );
        multiply->set_precedence( 2 );
        tokens.push_back( LexerToken(TOKEN_LITERAL, 3, multiply, "*") );

        ParserSymbol* divide = grammar.add_symbol( SYMBOL_TERMINAL, "/", 3 );
        divide->set_associativity( ASSOCIATE_LEFT );
        divide->set_precedence( 2 );
        tokens.push_back( LexerToken(TOKEN_LITERAL, 3, divide, "/") );
        
        ParserSymbol* left_parenthesis = grammar.add_symbol( SYMBOL_TERMINAL, "(", 10 );
        tokens.push_back( LexerToken(TOKEN_LITERAL, 10, left_parenthesis, "(") );

        ParserSymbol* right_parenthesis = grammar.add_symbol( SYMBOL_TERMINAL, ")", 10 );
        tokens.push_back( LexerToken(TOKEN_LITERAL, 10, right_parenthesis, ")") );

        ParserSymbol* integer = grammar.add_symbol( SYMBOL_TERMINAL, "[0-9]+", 4 );
        integer->set_associativity( ASSOCIATE_NONE );
        integer->set_precedence( 3 );
        tokens.push_back( LexerToken(TOKEN_REGULAR_EXPRESSION, 4, integer, "[0-9]+") );
        
        whitespace_tokens.push_back( LexerToken(TOKEN_REGULAR_EXPRESSION, 15, NULL, "[ \t\r\n]*") );

        ParserSymbol* expr = grammar.add_symbol( SYMBOL_NON_TERMINAL, "expr", 6 );
        ParserAction* add_action = grammar.add_action( "add" );
        grammar.begin_production( expr, 0 );
        grammar.symbol( expr );
        grammar.symbol( plus );
        grammar.symbol( expr );
        grammar.action( add_action );
        grammar.end_production();
        
        ParserAction* subtract_action = grammar.add_action( "subtract" );
        grammar.begin_production( expr, 0 );
        grammar.symbol( expr );
        grammar.symbol( minus );
        grammar.symbol( expr );
        grammar.action( subtract_action );
        grammar.end_production();

        ParserAction* multiply_action = grammar.add_action( "multiply" );
        grammar.begin_production( expr, 0 );
        grammar.symbol( expr );
        grammar.symbol( multiply );
        grammar.symbol( expr );
        grammar.action( multiply_action );
        grammar.end_production();

        ParserAction* divide_action = grammar.add_action( "divide" );
        grammar.begin_production( expr, 0 );
        grammar.symbol( expr );
        grammar.symbol( divide );
        grammar.symbol( expr );
        grammar.action( divide_action );
        grammar.end_production();

        ParserAction* compound_action = grammar.add_action( "compound" );
        grammar.begin_production( expr, 0 );
        grammar.symbol( left_parenthesis );
        grammar.symbol( expr );
        grammar.symbol( right_parenthesis );
        grammar.action( compound_action );
        grammar.end_production();        

        ParserAction* integer_action = grammar.add_action( "integer" );
        grammar.begin_production( expr, 0 );
        grammar.symbol( integer );
        grammar.action( integer_action );
        grammar.end_production();     
    }   

    LexerStateMachine lexer_state_machine( "", tokens, whitespace_tokens );
    ParserStateMachine parser_state_machine( grammar );
    Lexer<const char*> lexer( &lexer_state_machine, parser_state_machine.end_symbol() );
    Parser<const char*, int> parser( &parser_state_machine );
    parser.parser_action_handlers()
        ( "add", &add )
        ( "subtract", &subtract )
        ( "multiply", &multiply )
        ( "divide", &divide )
        ( "compound", &compound )
        ( "integer", &integer )
    ;
    
    const char* input = "1 + 2 * (3 + 4) + 5";
    lexer.reset( input, input + strlen(input) );
    lexer.advance();
    while ( parser.parse(lexer.symbol(), lexer.lexeme()) )
    {
        lexer.advance();
    }
    SWEET_ASSERT( parser.accepted() );
    SWEET_ASSERT( lexer.full() );
    SWEET_ASSERT( parser.user_data() == 20 );
}
