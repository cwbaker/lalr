
#include <stdio.h>
#include <sweet/lexer/LexerStateMachine.hpp>
#include <sweet/lexer/Lexer.ipp>
#include <sweet/lexer/LexerToken.hpp>
#include <sweet/parser/ParserStateMachine.hpp>
#include <sweet/parser/ParserGrammar.hpp>
#include <sweet/parser/Parser.ipp>
#include <string.h>

using namespace std;
using namespace sweet;
using namespace sweet::parser;
using namespace sweet::parser;

static shared_ptr<ParserUserData<char> > hello_world( const ParserSymbol* /*symbol*/, const ParserNode<>* /*start*/, const ParserNode<>* /*finish*/ )
{
    printf( "Hello World!\n" );
    return shared_ptr<ParserUserData<char> >();
}

void parser_hello_world_with_grammar_example()
{
//
// hello_world {
//       hello_world: 'Hello World!' [hello_world];
// }
//
    ParserGrammar grammar;
    ParserSymbol* hello_world_non_terminal = grammar.add_symbol( SYMBOL_NON_TERMINAL, "hello_world", 1 );
    ParserAction* hello_world_action = grammar.add_action( "hello_world" );
    ParserSymbol* hello_world_terminal = grammar.add_symbol( SYMBOL_TERMINAL, "Hello World!", 1 );
    grammar.begin_production( hello_world_non_terminal, 0 );
    grammar.symbol( hello_world_terminal );
    grammar.action( hello_world_action );
    grammar.end_production();
        
    vector<LexerToken> tokens;
    vector<LexerToken> whitespace_tokens;
    tokens.push_back( LexerToken(TOKEN_LITERAL, 1, hello_world_terminal, "Hello World!") );

    LexerStateMachine lexer_state_machine( "", tokens );
    ParserStateMachine parser_state_machine( grammar );
    Lexer<const char*> lexer( &lexer_state_machine, parser_state_machine.end_symbol() );
    Parser<const char*> parser( &parser_state_machine );
    parser.parser_action_handlers()
        ( "hello_world", &hello_world )
    ;
    
    const char* input = "Hello World!";
    lexer.reset( input, input + strlen(input) );
    lexer.advance();
    while ( parser.parse(lexer.symbol(), lexer.lexeme()) )
    {
        lexer.advance();
    }
    SWEET_ASSERT( parser.accepted() );
    SWEET_ASSERT( lexer.full() );
}
