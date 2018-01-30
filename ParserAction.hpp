#ifndef SWEET_LALR_PARSERACTION_HPP_INCLUDED
#define SWEET_LALR_PARSERACTION_HPP_INCLUDED

#include <string>

namespace sweet
{

namespace lalr
{

/**
// An action that is attached to a %parser reduction.
*/
class ParserAction
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