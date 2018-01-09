//
// NullDeleter.hpp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_NULLDELETER_HPP_INCLUDED
#define SWEET_POINTER_NULLDELETER_HPP_INCLUDED

namespace sweet
{

namespace pointer
{

/**
// Do nothing deleter for use with ptr<>.
*/
template <class Type>
struct NullDeleter
{
    void operator()( Type* ) const
    {
    }
};

}

}

#endif