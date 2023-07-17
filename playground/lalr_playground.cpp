#include <emscripten.h>
#include <stdio.h>
#include <string.h>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.hpp>
#include <time.h>

EM_JS(void, switch_output, (const char* which), {
    output = UTF8ToString(which);
})

EM_JS(void, set_result, (const char* which, int value), {
    result_name = UTF8ToString(which);
    result[result_name] = value;
})

EM_JS(void, showDiffTime, (const char *title), {
    const now = new Date().getTime();
    const diff_time = now - parse_start_time;

    outputs.parse_time  += UTF8ToString(title) + " -> Time taken : " + diff_time + "ms<br>\n";
    parse_start_time = now;
});

typedef unsigned char mychar_t;

struct C_MultLineCommentLexer
{
	static lalr::PositionIterator<const mychar_t*> string_lexer( const lalr::PositionIterator<const mychar_t*>& begin,
							const lalr::PositionIterator<const mychar_t*>& end,
							std::basic_string<mychar_t>* lexeme,
							const void** /*symbol*/ )
	{
		LALR_ASSERT( lexeme );

		lexeme->clear();
                //printf("C_MultLineCommentLexer : %s\n", lexeme->c_str());

		bool done = false;
		lalr::PositionIterator<const mychar_t*> i = begin;
		while ( i != end && !done)
		{
			switch( *i )
			{
				case '*':
					++i;
					if(i != end && *i == '/') done = true;
					continue;
					break;
			}
			++i;
		}
		if ( i != end )
		{
			LALR_ASSERT( *i == '/' );
			++i;
		}
		return i;
	}
};


struct ParseTreeUserDataDbg {
    int index;
    int stack_index;
    static int next_index;
    static int total;
    ParseTreeUserDataDbg():index(total++), stack_index(next_index++) {};
};
int ParseTreeUserDataDbg::next_index = 0;
int ParseTreeUserDataDbg::total = 0;


static bool parseTreeMakerDbg( ParseTreeUserDataDbg& result, const ParseTreeUserDataDbg* start, const lalr::ParserNode<mychar_t>* nodes, size_t length )
{
   switch_output("ast");
//    //printf("astMaker: %s\n", nodes[0].lexeme().c_str());
//    const char *lexstr = (length > 0 ? (const char *)nodes[0].lexeme().c_str() : "::lnull");
//    const char *idstr = (length > 0 ? nodes[0].symbol()->identifier : "::inull");
//    int line = (length > 0 ? nodes[0].line() : 0);
//    int column = (length > 0 ? nodes[0].column() : 0);
//    //const char *stateLabel = (length > 0 ? nodes[0].state()->label : "::inull");
//    printf("astMaker: %p\t%zd:%d:%d\t%p\t%zd\t->\t%s : %s :%d:%d\n", start, length,
//                length ? start->index : -1, length ? start->stack_index : -1,
//                nodes, length, idstr, lexstr, line, column);
    printf("----\n");
    for(size_t i=0; i< length; ++i)
        printf("%zd:%d\t%p\t%d:%d\t%p <:> %s <:> %s <:> %s <:> %d:%d\n", i, nodes[i].symbol()->type,
                start+i, start[i].index, start[i].stack_index, nodes+i,
                nodes[i].symbol()->identifier, nodes[i].symbol()->lexeme,
                nodes[i].lexeme().c_str(), nodes[i].line(), nodes[i].column());
    switch_output("parse_status");
    return true;
}


struct ParseTreeUserData {
    std::vector<ParseTreeUserData> children;
    const lalr::ParserSymbol *symbol;
    std::basic_string<mychar_t> lexeme; ///< The lexeme at this node (empty if this node's symbol is non-terminal).
    ParseTreeUserData():children(0),symbol(nullptr) {};
};


static bool parsetreeMaker( ParseTreeUserData& result, const ParseTreeUserData* start, const lalr::ParserNode<mychar_t>* nodes, size_t length )
{
    if(length == 0) return false;
    result.symbol = nodes[length-1].state()->transitions->reduced_symbol;
    for(size_t i_node = 0; i_node < length; ++i_node)
    {
        const lalr::ParserNode<mychar_t>& the_node = nodes[i_node];
        switch(the_node.symbol()->type)
        {
            case lalr::SymbolType::SYMBOL_TERMINAL:
            {
                ParseTreeUserData& udt = result.children.emplace_back();
                udt.symbol = the_node.symbol();
                udt.lexeme = the_node.lexeme();
                //printf("TERMINAL: %s : %s\n", udt.symbol->identifier, udt.lexeme.c_str());
            }
            break;
            case lalr::SymbolType::SYMBOL_NON_TERMINAL:
            {
                if(the_node.symbol() == result.symbol)
                {
                    const ParseTreeUserData& startx = start[i_node];
                    for (std::vector<ParseTreeUserData>::const_iterator child = startx.children.begin(); child != startx.children.end(); ++child)
                    {
                        result.children.push_back( std::move(*child) );
                    }
                }
                else
                {
                    ParseTreeUserData& udt = result.children.emplace_back();
                    udt.symbol = the_node.symbol();
                    if(udt.symbol == start[i_node].symbol)
                    {
                        udt.children = start[i_node].children;
                    }
                    else
                        udt.children.push_back(std::move(start[i_node]));
                }
                //printf("NON_TERMINAL: %s\n", result.symbol->identifier);
            }
            break;
            default:
                //LALR_ASSERT( ?? );
                printf("Unexpected symbol %p\n", the_node.symbol());
        }
    }
    return true;
}

static void indent( int level )
{
    for ( int i = 0; i < level; ++i )
    {
        printf( " |" );
    }
}

static void print_parsetree( const ParseTreeUserData& ast, int level )
{
    if(ast.symbol)
    {
        indent( level );
        switch(ast.symbol->type)
        {
            case lalr::SymbolType::SYMBOL_TERMINAL:
                if(ast.lexeme.size())
                {
                    //indent( level -1);
                    printf("%s -> %s\n", ast.symbol->identifier, ast.lexeme.c_str());
                }
                break;
            case lalr::SymbolType::SYMBOL_NON_TERMINAL:
                //indent( level );
                printf("%s\n", ast.symbol->lexeme);
                break;
        }
    }

    for (std::vector<ParseTreeUserData>::const_iterator child = ast.children.begin(); child != ast.children.end(); ++child)
    {
        print_parsetree( *child, ast.symbol ? (level + 1) : level );
    }
}

extern "C" int parse(const char *grammar, const unsigned char *input, int dumpLexer, int generate_ebnf, int generate_yacc, int generate_parsetree)
{
    int err = 0; // currently, zero is always returned; result codes for each part
                 // are sent to JS via set_result()

    // default result values (-1 indicates we didn't reach a part)
    int compile_result = -1;
    int parse_result = -1;
    int ast_result = -1;
    int profile_result = -1;

    // baked-in verbose levels
    const int compile_verbose = 0;
    const int parse_verbose = 0;

    //
    // Compile Grammar
    //

    switch_output("compile_status");

    lalr::GrammarCompiler compiler;
    lalr::ErrorPolicy error_policy;
    int errors = compiler.compile( grammar, grammar + strlen(grammar), &error_policy,  generate_ebnf, generate_yacc);

    if (errors != 0) {
        fprintf(stderr, "Error compiling grammar. "
            "Error count = %d\n", errors);
	err = -1;
        goto done;
    }
    else {
	if(generate_ebnf || generate_yacc) {
	    err = -3;
	    parse_result = 0;
	    goto done;
	}
        if (compile_verbose) {
            fprintf(stderr, "Grammar compiled successfully.\n");
        }
	showDiffTime("Compile grammar");
    }

    switch_output("parse_stats");
    compiler.showStats();

    //
    // Parse input
    //

    switch_output("parse_status");

    if(input) {
        lalr::ErrorPolicy error_policy_input;
	if(generate_parsetree == 1)
	{
		lalr::Parser<const mychar_t*, ParseTreeUserData> parser( compiler.parser_state_machine(), &error_policy_input );
		parser.set_default_action_handler(parsetreeMaker);
		parser.lexer_action_handlers()
		    ( "C_MultilineComment", &C_MultLineCommentLexer::string_lexer )
		;
		if(dumpLexer) {
			switch_output("lexer");
			parser.dumpLex( input, input + strlen((const char*)input) );
		}
		else parser.parse(  input, input + strlen((const char*)input) );

		parse_result = parser.accepted() == 1 && parser.full() == 1;
		if(generate_parsetree && parse_result)
		{
			showDiffTime("Parse input");
			switch_output("ast");
			print_parsetree( parser.user_data(), 0 );
			switch_output("parse_status");
			showDiffTime("Print parse tree");
		}
	}
	if(generate_parsetree > 1)
	{
		lalr::Parser<const mychar_t*, ParseTreeUserDataDbg> parser( compiler.parser_state_machine(), &error_policy_input );
		parser.set_default_action_handler(parseTreeMakerDbg);
		parser.lexer_action_handlers()
		    ( "C_MultilineComment", &C_MultLineCommentLexer::string_lexer )
		;
		if(dumpLexer) {
			switch_output("lexer");
			parser.dumpLex( input, input + strlen((const char*)input) );
		}
		else parser.parse(  input, input + strlen((const char*)input) );

		parse_result = parser.accepted() == 1 && parser.full() == 1;
	}
	else
	{
		lalr::Parser<const mychar_t*, int> parser( compiler.parser_state_machine(), &error_policy_input );
		parser.lexer_action_handlers()
		    ( "C_MultilineComment", &C_MultLineCommentLexer::string_lexer )
		;
		if(dumpLexer) {
			switch_output("lexer");
			parser.dumpLex( input, input + strlen((const char*)input) );
		}
		else parser.parse(  input, input + strlen((const char*)input) );

		parse_result = parser.accepted() == 1 && parser.full() == 1;
	}
        if (parse_result) {
            if (parse_verbose) {
                fprintf(stderr, "Parse successful.\n");
            }
	     if(generate_parsetree != 1)
                 showDiffTime("Parse input");
        }
        else {
     	    err = -2;
            goto done;
        }
    }


done:

    set_result("compile", errors);
    set_result("parse", parse_result ? 0 : err);

    return err;
}
