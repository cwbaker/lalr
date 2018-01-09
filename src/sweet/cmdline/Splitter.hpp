#ifndef SWEET_CMDLINE_SPLITTER_HPP_INCLUDED
#define SWEET_CMDLINE_SPLITTER_HPP_INCLUDED

#include "declspec.hpp"
#include <string>
#include <vector>

namespace sweet
{

namespace cmdline
{

/**
// Split a string into an array of const char* suitable for passing to 
// exec(), posix_spawn(), etc.
*/
class SWEET_CMDLINE_DECLSPEC Splitter
{
    std::string command_line_;
    std::vector<char*> arguments_;

public:
    Splitter( const char* command_line );
    const std::vector<char*>& arguments();
};

}

}

#endif
