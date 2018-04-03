#ifndef LALR_ASSERT_HPP_INCLUDED
#define LALR_ASSERT_HPP_INCLUDED

#include <assert.h>

#if !defined LALR_ASSERT && !defined NDEBUG
#define LALR_ASSERT( condition ) assert((condition))
#else
#define LALR_ASSERT( condition ) ((void) 0)
#endif

#endif
