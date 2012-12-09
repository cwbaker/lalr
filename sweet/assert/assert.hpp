//
// assert.hpp
// Copyright (c) 2006 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ASSERT_ASSERT_HPP_INCLUDED
#define SWEET_ASSERT_ASSERT_HPP_INCLUDED

#if defined(BUILD_MODULE_ASSERT) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_ASSERT_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_ASSERT_DECLSPEC __declspec(dllimport)
#else
#define SWEET_ASSERT_DECLSPEC
#endif 

#include <sweet/build.hpp>

#ifndef BUILD_MODULE_ASSERT
#pragma comment( lib, "assert" BUILD_LIBRARY_SUFFIX )
#endif

namespace sweet
{

/**
 Assertion library.

 The assert component provides the macro SWEET_ASSERT(<em>e</em>) for 
 asserting that assumptions are true at runtime. The macro compiles to code
 to generate a __debugbreak() call if the expression is false when the macro
 SWEET_ASSERT_ENABLED is defined and to nothing otherwise.

 An assert function (sweet_assert()) is provided for situations where a macro
 doesn't work.  Lua code uses assertions in expressions in a way that requires
 they be in a function. 
*/
namespace assert
{

SWEET_ASSERT_DECLSPEC void sweet_break();
SWEET_ASSERT_DECLSPEC void sweet_assert( bool expression, const char* description, const char* file, int line );

}

}

#ifdef _MSC_VER
#define SWEET_BREAK() __debugbreak()
#else
#define SWEET_BREAK() sweet::assert::sweet_break()
#endif

#ifdef SWEET_ASSERT_ENABLED

#define SWEET_ASSERT( x ) \
do { \
    if ( !(x) ) \
    { \
        sweet::assert::sweet_assert( false, #x, __FILE__, __LINE__ ); \
        SWEET_BREAK(); \
    } \
} while ( false )

#else

#pragma warning( disable: 4127 )
#define SWEET_ASSERT( x )

#endif

#endif
