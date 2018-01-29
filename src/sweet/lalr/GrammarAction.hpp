#ifndef SWEET_PARSER_GRAMMARACTION_HPP_INCLUDED
#define SWEET_PARSER_GRAMMARACTION_HPP_INCLUDED

#include <string>

namespace sweet
{

namespace lalr
{

class GrammarAction
{
    std::string identifier_;

public:
    GrammarAction( std::string identifier );
    const std::string& identifier() const;
};

}

}

#endif
