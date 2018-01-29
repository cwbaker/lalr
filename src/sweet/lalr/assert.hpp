#ifndef SWEET_PARSER_ASSERT_HPP_INCLUDED
#define SWEET_PARSER_ASSERT_HPP_INCLUDED

#include <assert.h>

#if !defined SWEET_ASSERT && !defined NDEBUG
#define SWEET_ASSERT( condition ) assert((condition))
#else
#define SWEET_ASSERT( condition ) ((void) 0)
#endif

#endif
