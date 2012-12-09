//
// Parser.cpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#include "Parser.hpp"
#include "Error.hpp"
#include <sweet/assert/assert.hpp>
#include <stdio.h>
#include <stdlib.h>

using std::vector;
using namespace sweet::cmdline;

/**
// Constructor.
*/
Parser::Parser()
: options_(),
  operands_( 0 )
{
}

void Parser::parse( int argc, char** argv ) const
{
    parse( argc, const_cast<const char**>(argv) );
}

/**
// Parse a command line using the Options that have been added to this 
// Parser.
//
// @param argc
//  The number of command line arguments.
//
// @param argv
//  The command line arguments.
//
// @return
//  True if parsing the command line succeeded otherwise false.
*/
void Parser::parse( int argc, const char** argv ) const
{
    int argi = 1;
    while ( argi < argc )
    {
        if ( is_short_option(argv[argi]) )
        {
            bool          option_parsed = false;
            const char*   argument      = argv[argi] + 1;
            const Option* option        = find_option_by_short_name( std::string(argument, argument + 1) );

            while ( option != 0 && option->get_type() == OPTION_BOOL )
            {
                option_parsed = true;
                ++argument;
                parse_option( option, argument, argv[argi + 1] );
                option = find_option_by_short_name( std::string(argument, argument + 1) );
            }

            if ( option != 0 )
            {
                if ( option_parsed )
                {
                    SWEET_ERROR( InvalidOptionError("Option '-%c' is invalid as part of a group because it requires an argument", *argument) );
                }

                option_parsed = true;
                ++argument;
                argi += parse_option( option, argument, argv[argi + 1] );
            }

            if ( !option_parsed )
            {
                SWEET_ERROR( InvalidOptionError("Unrecognized option '-%c'", *argument) );
            }
        }
        else if ( is_long_option(argv[argi]) )
        {
            const char* name     = argv[argi] + 2;
            const char* name_end = find_end_of_name( name );
            const char* argument = find_argument( name_end );

            const Option* option = find_option_by_name( std::string(name, name_end) );
            if ( option != 0 )
            {
                argi += parse_option( option, argument, argv[argi + 1] );
            }
            else
            {
                SWEET_ERROR( InvalidOptionError("Unrecognized option '--%s'", name) );
            }
        }
        else
        {
            if ( operands_ != 0 )
            {
                operands_->push_back( argv[argi] );
            }            
        }

        ++argi;
    }
}

/**
// Print the Options that this Parser recognises to a stream.
//
// @param stream
//  The stream to print Options to (assumed to be not null).
//
// @param width
//  The width to pad long option names to (in characters).
//
// @return
//  Nothing.
*/
void Parser::print( FILE* stream, int width ) const
{
    SWEET_ASSERT( stream );

    for ( vector<Option>::const_iterator option = options_.begin(); option != options_.end(); ++option )
    {
        fprintf( stream, "  " );

        if ( !option->get_short_name().empty() )
        {
            fprintf( stream, "-%s", option->get_short_name().c_str() );
        }

        if ( !option->get_name().empty() )
        {
            if ( !option->get_short_name().empty() )
            {
                fprintf( stream, ", " );
            }

            fprintf( stream, "--%-*s", width, option->get_name().c_str() );
        }

        fprintf( stream, " %s.", option->get_description().c_str() );
        fprintf( stream, "\n" );
    }
}

/**
// Add Options to this Parser.
//
// @return
//  An AddOption helper object that provides a convenient syntax for adding
//  Options to this Parser.
*/
AddOption Parser::add_options()
{
    return AddOption( &options_, &operands_ );
}

/**
// Is a command line argument a short option?
//
// @param argument
//  The argument to check for being a short option or not.
//
// @return
//  True if \e argument is a short option otherwise false.
*/
bool Parser::is_short_option( const char* argument ) const
{
    SWEET_ASSERT( argument != 0 );
    return argument[0] == '-' && argument[1] != '-';
}

/**
// Is a command line argument a long option?
//
// @param argument
//  The argument to check for being a long option or not.
//
// @return
//  True if \e argument is a long option otherwise false.
*/
bool Parser::is_long_option( const char* argument ) const
{
    SWEET_ASSERT( argument != 0 );
    return argument[0] == '-' && argument[1] == '-';
}

/**
// Find an Option by name.
//
// @param name
//  The name of the Option to find.
//
// @return
//  The Option or null if no matching Option was found.
*/
const Option* Parser::find_option_by_name( const std::string& name ) const
{
    vector<Option>::const_iterator option = options_.begin();
    while ( option != options_.end() && option->get_name() != name )
    {
        ++option;
    }

    return option != options_.end() ? &(*option) : 0;
}

/**
// Find an Option by its short name.
//
// @param short_name
//  The short name of the Option to find.
//
// @return
//  The Option or null if no matching Option was found.
*/ 
const Option* Parser::find_option_by_short_name( const std::string& short_name ) const
{
    vector<Option>::const_iterator option = options_.begin();
    while ( option != options_.end() && option->get_short_name() != short_name )
    {
        ++option;
    }

    return option != options_.end() ? &(*option) : 0;
}

/**
// Find the end of a name in a command line argument.
//
// The end of the name is at the first '=' or null character that is 
// encountered after \e name.
//
// @param name
//  The name to find the end of (assumed not to be null).
//
// @return 
//  The end of the name in \e name.
*/
const char* Parser::find_end_of_name( const char* name ) const
{
    SWEET_ASSERT( name );

    while ( *name != 0 && *name != '=' )
    {
        ++name;
    }

    return name;
}

/**
// Find the beginning of the argument (if there is one) in a command line
// argument.
//
// The argument is the first character following the first '=' character that
// is encountered after \e name_end.
//
// @param name_end
//  The end of the name to find the argument after (assumed not to be null).
//
// @return
//  The beginning of the argument after \e name_end.
*/
const char* Parser::find_argument( const char* name_end ) const
{
    SWEET_ASSERT( name_end );

    if ( *name_end != '=' )
    {
        ++name_end;
    }

    return name_end;
}

/**
// Parse an Option.
//
// @param option
//  The Option to parse (assumed not to be null).
//
// @param argument
//  The command line argument that contained the Option and may also contain
//  the optional argument for the Option (assumed not to be null).
//
// @param next_argument
//  The command line argument that follows the argument containing the Option 
//  that may also contain the optional argument for the Option (assumed not to
//  be null).
//
// @return
//  The number of arguments that the higher level parser should skip (0 if
//  the Option had no arguments or arguments were contained within the 
//  argument that the Option was in otherwise 1 because the Option had an 
//  argument that was contained within the argument following the argument 
//  that the Option was in).
*/
int Parser::parse_option( const Option* option, const char* argument, const char* next_argument ) const
{
    SWEET_ASSERT( option );
    SWEET_ASSERT( option->get_address() );
    SWEET_ASSERT( argument );

//
// If the Option is not a boolean option then it must have an argument that
// follows it either contained in the same command line argument as the 
// option was in (e.g. -j3 or --jobs=3) or in the command line argument that
// follows it (e.g. -j 3 or --jobs 3).  So skip the optional leading '=' and
// skip to the command line argument that follows if the command line argument
// that contains the option doesn't also contain an argument.
//
    if ( option->get_type() != OPTION_BOOL )
    {
        if ( *argument == '=' )
        {
            ++argument;
        }

        if ( *argument == 0 )
        {
            argument = next_argument;
        }

        if ( argument == 0 )
        {
            SWEET_ERROR( InvalidArgumentError("Option '--%s' requires an argument", option->get_name().c_str()) );
        }
    }

//
// Extract the argument value for the Option - true for boolean options or 
// the value of the argument for other option types.
//
    switch ( option->get_type() )
    {
        case OPTION_BOOL:
        {
            bool* address = static_cast<bool*>( option->get_address() );
            *address = true;
            break;
        }

        case OPTION_INT:
        {
            int* address = static_cast<int*>( option->get_address() );
            *address = ::atoi( argument );
            break;
        }

        case OPTION_FLOAT:
        {
            float* address = static_cast<float*>( option->get_address() );
            *address = static_cast<float>( ::atof(argument) );
            break;
        }

        case OPTION_STRING:
        {
            std::string* address = static_cast<std::string*>( option->get_address() );
            *address = argument;
            break;
        }

        default:
            SWEET_ASSERT( false );
            break;
    }

    return option->get_type() != OPTION_BOOL && argument == next_argument ? 1 : 0;
}
