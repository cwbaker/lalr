#pragma once

#include <memory>
#include <iterator>
#include <string>

namespace lalr
{

template <class Iterator, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char>>
void line_comment( Iterator begin, Iterator end, std::basic_string<Char, Traits, Allocator>* lexeme, const void** /*symbol*/, Iterator* position, int* lines )
{
    LALR_ASSERT( begin );
    LALR_ASSERT( end );
    LALR_ASSERT( begin <= end );
    LALR_ASSERT( lexeme );
    LALR_ASSERT( position );
    LALR_ASSERT( lines );
    
    lexeme->clear();
    
    Iterator i = begin;
    while ( i != end && *i != '\n' && *i != '\r' )
    {
        ++i;
    }
    
    *position = i;
    *lines = 1;
}

}