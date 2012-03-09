//
// ParserAction.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef PARSERACTION_HPP_INCLUDED
#define PARSERACTION_HPP_INCLUDED

#include "declspec.hpp"
#include <string>

namespace sweet
{

namespace parser
{

/**
// An action that is attached to a %parser reduction.
*/
class SWEET_PARSER_DECLSPEC ParserAction
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        int index_; ///< The index of this action.
        std::string identifier_; ///< The identifier of this action.
    
    public:
        ParserAction( int index, const std::string& identifier );
        int get_index() const;
        const std::string& get_identifier() const;
};

}

}

#endif