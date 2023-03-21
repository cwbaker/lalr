#ifndef LALR_POSITIONITERATOR_HPP_INCLUDED
#define LALR_POSITIONITERATOR_HPP_INCLUDED

#include "assert.hpp"
#include <iterator>

namespace lalr
{

/**
// An iterator wrapper that adds line number tracking to iterators or 
// pointers.
*/
template <class Iterator>
class PositionIterator
{
public:
    typedef std::forward_iterator_tag iterator_category; ///< The type of this iterator (always forward regardless of the underlying iterator).
    typedef typename std::iterator_traits<Iterator>::difference_type difference_type; ///< The type to represent the difference between two iterators.
    typedef typename std::iterator_traits<Iterator>::value_type value_type; ///< The type of the element the iterator can point to.
    typedef typename std::iterator_traits<Iterator>::pointer pointer; ///< The type of a pointer to an element the iterator can point to.
    typedef typename std::iterator_traits<Iterator>::reference reference; ///< Type to represent a reference to an element pointed by the iterator.

private:
    Iterator position_; ///< The current position of this iterator.
    Iterator end_; ///< One past the last position of the input sequence for this iterator.
    bool ended_; ///< True if this iterator has reached its end.
    int line_; ///< The current line number of this iterator.
    int column_; ///< The current column number of this iterator.

public:
    PositionIterator()
    : position_()
    , end_()
    , ended_( true )
    , line_( 1 )
    , column_( 1 )
    {
    }

    PositionIterator( Iterator begin, Iterator end )
    : position_( begin )
    , end_( end )
    , ended_( begin == end )
    , line_( 1 )
    , column_( 1 )
    {
    }        
    
    PositionIterator( const PositionIterator& iterator )
    : position_( iterator.position_ )
    , end_( iterator.end_ )
    , ended_( iterator.ended_ )
    , line_( iterator.line_ )
    , column_(iterator.column_)
    {
    }
            
    PositionIterator& operator=( const PositionIterator& iterator )
    {
        if ( this != &iterator )
        {
            position_ = iterator.position_;
            end_ = iterator.end_;
            ended_ = iterator.ended_;
            line_ = iterator.line_;
            column_ = iterator.column_;
        }
        
        return *this;
    }
            
    PositionIterator& operator++()
    {
        int character = *position_;
        ++position_;
        ++column_;

        if ( character == '\n' )
        {
            ++line_;
            column_ = 1;
        }
        else if ( character == '\r' )
        {
            if ( position_ != end_ && *position_ == '\n' )
            {
                ++position_;
            }
            ++line_;
            column_ = 1;
        } 
        
        ended_ = position_ == end_;
        return *this;
    }
            
    value_type operator*() const
    {
        return *position_;
    }

    bool ended() const
    {
        return ended_;
    }

    const Iterator& position() const
    {
        return position_;
    }

    const Iterator& end() const
    {
        return end_;
    }

    int line() const
    {
        return line_;
    }

    int column() const
    {
        return column_;
    }
            
    bool operator!=( const PositionIterator& iterator ) const
    {
        return ended_ || iterator.ended_ ? ended_ != iterator.ended_ : position_ != iterator.position_;
    }
            
    bool operator==( const PositionIterator& iterator ) const
    {
        return ended_ || iterator.ended_ ? ended_ == iterator.ended_ : position_ == iterator.position_;
    }

    void skip( Iterator position, int lines )
    {
        LALR_ASSERT( lines >= 0 );
        position_ = position;
        ended_ = position_ == end_;
        line_ += lines;
        column_ = 1;
    }
};

}

#endif
