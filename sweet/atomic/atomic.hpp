//
// atomic.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ATOMIC_ATOMIC_HPP_INCLUDED
#define SWEET_ATOMIC_ATOMIC_HPP_INCLUDED

namespace sweet
{

/**
// Atomic operations library.
*/
namespace atomic
{

inline int atomic_increment( volatile int* destination );
inline int atomic_decrement( volatile int* destination );
inline int atomic_exchange( volatile int* destination, int exchange );
inline int atomic_compare_exchange( volatile int* destination, int exchange, int comparand );
inline int current_thread_id();

}

}

#endif
