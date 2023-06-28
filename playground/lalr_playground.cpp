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

struct ParseTreeUserData {
    std::vector<ParseTreeUserData> children;
    const lalr::ParserSymbol *symbol;
    std::basic_string<mychar_t> lexeme; ///< The lexeme at this node (empty if this node's symbol is non-terminal).
    ParseTreeUserData():children(0),symbol(nullptr) {};
};


static bool parsetreeMaker( ParseTreeUserData& result, const ParseTreeUserData* start, const lalr::ParserNode<mychar_t>* nodes, size_t length )
{
    if(length == 2 && nodes[0].symbol()->type == lalr::SymbolType::SYMBOL_NON_TERMINAL && nodes[1].symbol()->type == lalr::SymbolType::SYMBOL_NON_TERMINAL)
    {
        if(start[0].symbol == nodes[0].symbol())
        {
            result = start[0];
        }
        else
        {
            result.symbol = nodes[0].symbol();
        }
        ParseTreeUserData& udt = result.children.emplace_back();
        udt.symbol = nodes[1].symbol();
        udt.children.push_back(start[1]);
    }
    else
    {
        for(size_t i_node = 0; i_node < length; ++i_node)
        {
            const lalr::ParserNode<mychar_t>& the_node = nodes[i_node];
            switch(the_node.symbol()->type)
            {
                case lalr::SymbolType::SYMBOL_TERMINAL:
                {
                    ParseTreeUserData& udt = (length == 1) ? result : result.children.emplace_back();
                    udt.symbol = the_node.symbol();
                    udt.symbol = the_node.symbol();
                    udt.lexeme = the_node.lexeme();
                    //printf("TERMINAL: %s : %s\n", udt.symbol->identifier, udt.lexeme.c_str());
                }
                    break;
                case lalr::SymbolType::SYMBOL_NON_TERMINAL:
                {
                    ParseTreeUserData& udt = (length == 1) ? result : result.children.emplace_back();
                    udt.symbol = the_node.symbol();
                    udt.children.push_back(start[i_node]);
                    //printf("NON_TERMINAL: %s\n", result.symbol->identifier);
                }
                    break;
                default:
                    //LALR_ASSERT( ?? );
                    printf("Unexpected symbol %p\n", the_node.symbol());
            }
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

extern "C" int parse(const char *grammar, const unsigned char *input, int dumpLexer, int generate_ebnf, int generate_parsetree)
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
    int errors = compiler.compile( grammar, grammar + strlen(grammar), &error_policy,  generate_ebnf != 0);

    if (errors != 0) {
        fprintf(stderr, "Error compiling grammar. "
            "Error count = %d\n", errors);
	err = -1;
        goto done;
    }
    else {
	if(generate_ebnf) {
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
        //lalr::Parser<const mychar_t*, int> parser( compiler.parser_state_machine(), &error_policy_input );
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
		switch_output("ast");
		print_parsetree( parser.user_data(), 0 );
		switch_output("parse_status");
	}
        if (parse_result) {
            if (parse_verbose) {
                fprintf(stderr, "Parse successful.\n");
            }
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
