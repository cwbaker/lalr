//
// cmdline.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_CMDLINE_HPP_INCLUDED
#define SWEET_CMDLINE_HPP_INCLUDED

#ifndef BUILD_MODULE_CMDLINE
#pragma comment( lib, "cmdline" BUILD_LIBRARY_SUFFIX )
#endif

namespace sweet
{

/**
 Command line parsing library.
 
 Parses arguments from the command line as specified by the POSIX and GNU
 standards.  See http://www.informit.com/articles/article.aspx?p=175771.

 Usage is to construct a Parser object on the stack, add command line options
 to it, and then parse command lines (as argc/argv pairs). 

@code
bool        help    = false;
bool        version = false;
bool        quiet   = false;
int         integer = 0;
float       real    = 0.0f;
std::string string  = "";
std::vector<std::string> input_files;

cmdline::Parser command_line_parser;
command_line_parser.add_options()
    ( "help",    "h", "Print this message and exit",           &help    )
    ( "version", "v", "Print the version and exit",            &version )
    ( "quiet",   "q", "Prevent the output of status messages", &quiet   )
    ( "integer", "i", "An integer argument",                   &integer )
    ( "real",    "r", "A real argument",                       &real    )
    ( "string",  "s", "A string argument",                     &string  )
    ( &input_files )
;

command_line_parser.parse( argc, argv );
@endcode

 The Parser is also able to generate a help message that displays all of the
 options that have been added to it along with their descriptions.

@code
if ( help )
{
    std::cout << " \n";
    std::cout << "Usage: example [options] [files] \n";
    std::cout << "Options: \n";
    command_line_parser.print( stdout );
}
@endcode
*/
namespace cmdline
{
}

}

#include "Error.hpp"
#include "Option.hpp"
#include "AddOption.hpp"
#include "Parser.hpp"

#endif
