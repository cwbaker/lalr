#pragma once

#include <memory>
#include <iterator>
#include <string>

namespace lalr
{

template <class Iterator, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char>>
PositionIterator<Iterator> block_comment( const PositionIterator<Iterator>& begin, const PositionIterator<Iterator>& end, std::basic_string<Char, Traits, Allocator>* lexeme, const void** /*symbol*/ )
{
    LALR_ASSERT( lexeme );
    lexeme->clear();
                    
    bool done = false;
    PositionIterator<Iterator> i = begin;
    while ( i != end && !done )
    {
        while ( i != end && *i != '*' )
        {
            ++i;
        }

        if ( i != end )
        {
            LALR_ASSERT( *i == '*' );

            ++i;
            if ( *i == '/' )
            {
                ++i;
                done = true;
            }
        }        
    }    
    return i;
}

}
