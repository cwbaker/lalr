//
// declspec.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_DEBUG_DECLSPEC_HPP_INCLUDED
#define SWEET_DEBUG_DECLSPEC_HPP_INCLUDED

#if defined(BUILD_MODULE_DEBUG) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_DEBUG_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_DEBUG_DECLSPEC __declspec(dllimport)
#else
#define SWEET_DEBUG_DECLSPEC
#endif 

#endif