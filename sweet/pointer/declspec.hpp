//
// declspec.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_DECLSPEC_HPP_INCLUDED
#define SWEET_POINTER_DECLSPEC_HPP_INCLUDED

#if defined(BUILD_MODULE_POINTER) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_POINTER_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_POINTER_DECLSPEC __declspec(dllimport)
#else
#define SWEET_POINTER_DECLSPEC
#endif 

#endif
