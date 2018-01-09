//
// LexerAction.hpp
// Copyright (c) 2009 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LEXER_LEXERACTION_HPP_INCLUDED
#define SWEET_LEXER_LEXERACTION_HPP_INCLUDED

#include "declspec.hpp"
#include <string>

namespace sweet
{

namespace lexer
{

/**
// An action that is attached to a lexical analyzer.
*/
class SWEET_LEXER_DECLSPEC LexerAction
{
    public:
        static const int INVALID_INDEX = -1;

    private:
        int         index_;      ///< The index of this action.
        std::string identifier_; ///< The identifier of this action.
    
    public:
        LexerAction( int index, const std::string& identifier );
        int get_index() const;
        const std::string& get_identifier() const;
};

}

}

#endif
