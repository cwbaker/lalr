#pragma once

#include <memory>
#include <iterator>
#include <string>

namespace lalr
{

template <class Iterator, class Char = typename std::iterator_traits<Iterator>::value_type, class Traits = typename std::char_traits<Char>, class Allocator = typename std::allocator<Char>>
PositionIterator<Iterator> string_literal( const PositionIterator<Iterator>& begin, const PositionIterator<Iterator>& end, std::basic_string<Char, Traits, Allocator>* lexeme, const void** /*symbol*/ )
{
    LALR_ASSERT( lexeme );
    LALR_ASSERT( lexeme->length() == 1 );

    int terminator = lexeme->at( 0 );
    LALR_ASSERT( terminator == '\'' || terminator == '"' );
    lexeme->clear();
    
    PositionIterator<Iterator> i = begin;
    while ( *i != terminator && i != end )
    {
        if ( *i != '\\' )
        {
            *lexeme += *i;
            ++i;            
        }
        else
        {
            ++i;
            if ( i != end )
            {
                int character = *i;
                switch ( character )
                {
                    case 'a':
                        character = '\a';
                        break;

                    case 'b':
                        character = '\b';
                        break;

                    case 'f':
                        character = '\f';
                        break; 

                    case 'n':
                        character = '\n';
                        break; 

                    case 'r':
                        character = '\r';
                        break; 

                    case 't':
                        character = '\t';
                        break; 

                    case 'v':
                        character = '\v';
                        break;

                    default:
                        break; 
                }
                *lexeme += character;
                ++i;
            }
        }
    }

    if ( i != end )
    {
        ++i;
    }

    return i;
}

}
