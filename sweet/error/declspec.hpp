//
// declspec.hpp
// Copyright (c) 2001 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ERROR_DECLSPEC_HPP_INCLUDED
#define SWEET_ERROR_DECLSPEC_HPP_INCLUDED

#if defined(BUILD_MODULE_ERROR) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_ERROR_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_ERROR_DECLSPEC __declspec(dllimport)
#else
#define SWEET_ERROR_DECLSPEC
#endif 

#endif
