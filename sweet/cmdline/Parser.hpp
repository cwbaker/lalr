//
// Parser.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_CMDLINE_PARSER_HPP_INCLUDED
#define SWEET_CMDLINE_PARSER_HPP_INCLUDED

#include "cmdline.hpp"
#include "Option.hpp"
#include <string>
#include <vector>

namespace sweet
{

namespace cmdline
{

/**
// A command line options parser.
*/
class SWEET_CMDLINE_DECLSPEC Parser
{
    std::vector<Option> options_;   ///< The Options that this Parser will parse from the command line.
    std::vector<std::string>* operands_;  ///< The vector of strings that this Parser will parse operands from the command line into.

    public:
        Parser();

        AddOption add_options();

        void parse( int argc, char** argv ) const;
        void print( FILE* stream, int width = 12 ) const;

    private:
        bool is_short_option( const char* argument ) const;
        bool is_long_option( const char* argument ) const;

        const Option* find_option_by_name( const std::string& name ) const;
        const Option* find_option_by_short_name( const std::string& short_name ) const;

        const char* find_end_of_name( const char* name ) const;
        const char* find_argument( const char* name_end ) const;

        int parse_option( const Option* option, const char* argument, const char* next_argument ) const;
};

}

}

#endif
