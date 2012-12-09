
#include "Splitter.hpp"
#include <sweet/assert/assert.hpp>
#include <string.h>

using std::string;
using std::vector;
using namespace sweet::cmdline;

/**
// Constructor.
// 
// @param command_line
//  The string to split into arguments at whitespace characters while 
//  combining arguments surrounded by single or double quotes (assumed not
//  null).
*/
Splitter::Splitter( const char* command_line )
: command_line_( command_line ),
  arguments_()
{
    SWEET_ASSERT( command_line );

    const char DOUBLE_QUOTE = '"';
    const char SINGLE_QUOTE = '\'';
    const char BACKSLASH = '\\';

    bool ended = true;
    char quote = 0;
    unsigned int length = strlen( command_line );
    command_line_.insert( command_line_.end(), 0, length );
    char* destination = &command_line_[0];
    const char* i = command_line;
    const char* end = i + length;
    while ( i != end )
    {
        if ( !quote )
        {
            while ( i != end && isspace(*i) )
            {
                ++i;
            }
        }

        if ( i != end )
        {
            if ( *i == SINGLE_QUOTE || *i == DOUBLE_QUOTE )
            {
                quote = *i;
                ++i;
            }
        }

        if ( i != end && ended )
        {
            ended = false;
            arguments_.push_back( destination );
        }

        if ( i != end && quote )
        {
            while ( i != end && *i != quote )
            {
                if ( *i == BACKSLASH )
                {
                    ++i;
                }
                if ( i != end )
                {
                    *destination = *i;
                    ++i;
                    ++destination;
                }
            }
            quote = 0;
            if ( i != end )
            {
                ++i;
            }
        }

        while ( i != end && !quote && !isspace(*i) )
        {
            if ( *i == DOUBLE_QUOTE || *i == SINGLE_QUOTE )
            {
                quote = *i;
                ++i;
            }
            else 
            {
                if ( i != end && *i == BACKSLASH )
                {
                    ++i;
                }
                if ( i != end )
                {
                    *destination = *i;
                    ++i;
                    ++destination;
                }
            }
        }

        if ( !quote )
        {
            ended = true;
            *destination = 0;
            ++destination;
        }
    }

    arguments_.push_back( NULL );
}

/**
// Get the arguments that the command line was split into.
//
// @return
//  The arguments (including a NULL terminator as the last element as required
//  by posix_spawn() etc).
*/
const std::vector<char*>& Splitter::arguments()
{
    return arguments_;
}
