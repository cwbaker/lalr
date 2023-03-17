#pragma once

#include <memory>
#include <iterator>
#include <string>

namespace lalr
{

template <class Iterator, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char>>
PositionIterator<Iterator> line_comment( const PositionIterator<Iterator>& begin, const PositionIterator<Iterator>& end, std::basic_string<Char, Traits, Allocator>* lexeme, const void** /*symbol*/ )
{
    LALR_ASSERT( lexeme );    
    lexeme->clear();

    PositionIterator<Iterator> i = begin;
    while ( i != end && *i != '\n' && *i != '\r' )
    {
        ++i;
    }    
    return i;
}

}