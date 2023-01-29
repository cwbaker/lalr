
#include <stdio.h>
#include <stdarg.h>
#include <lalr/GrammarCompiler.hpp>
#include <lalr/Parser.ipp>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

static int errors_ = 0;

static void show_error( const char* format, ... )
{
    ++errors_;
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
}

int read_file(const char *fname, std::vector<char> &content)
{
        struct stat stat;
        int result = ::stat( fname, &stat );
        if ( result != 0 )
        {
            show_error( "Stat failed on '%s' - result=%d\n", fname, result );
            return EXIT_FAILURE;
        }

        FILE* file = fopen( fname, "rb" );
        if ( !file )
        {
            show_error( "Opening '%s' to read failed - errno=%d\n", fname, errno );
            return EXIT_FAILURE;
        }

        int size = stat.st_size;
        content.resize( size );
        int read = int( fread(&content[0], sizeof(uint8_t), size, file) );
        fclose( file );
        file = nullptr;
        if ( read != size )
        {
            show_error( "Reading grammar from '%s' failed - read=%d\n", fname, int(read) );
            return EXIT_FAILURE;
        }
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	const char *grammar_fn = nullptr;
	const char *input_fn = nullptr;

	std::vector<char> grammar_txt;
	std::vector<char> input_txt;

	if ( argc < 2 )
	{
		printf( "%s -g|--grammar grammar_fname -i|--input input_fname\n", argv[0] );
		printf( "\n" );
		return EXIT_FAILURE;
	}

	int argi = 1;
	while ( argi < argc )
	{
		if ( strcmp(argv[argi], "-g") == 0 || strcmp(argv[argi], "--grammar") == 0 )
		{
		    grammar_fn = argv[argi + 1];
		    argi += 2;
		}
		else if ( strcmp(argv[argi], "-i") == 0 || strcmp(argv[argi], "--input") == 0 )
		{
		    input_fn = argv[argi + 1];
		    argi += 2;
		}
	}

	if(grammar_fn != nullptr)
	{
		int rc = read_file(grammar_fn, grammar_txt);
		if(rc != EXIT_SUCCESS) return rc;
		printf("Grammar size = %d\n", (int)grammar_txt.size());
		lalr::GrammarCompiler compiler;
		lalr::ErrorPolicy error_policy;
		int errors = compiler.compile( &grammar_txt[0], &grammar_txt[0] + grammar_txt.size(), &error_policy );
		if(errors != 0)
		{
			printf("Error count = %d\n", errors);
			return EXIT_FAILURE;
		}
		if(input_fn != nullptr)
		{
			rc = read_file(input_fn, input_txt);
			if(rc != EXIT_SUCCESS) return rc;
			printf("Input size = %d\n", (int)input_txt.size());
			lalr::ErrorPolicy error_policy_input;
			lalr::Parser<const char*, int> parser( compiler.parser_state_machine(), &error_policy_input );
			parser.parse( &input_txt[0], &input_txt[0] + input_txt.size() );
			printf( "accepted = %d, full = %d\n", parser.accepted(),  parser.full());
		}
	}
	return EXIT_SUCCESS;
}
