//
// declspec.hpp
// Copyright (c) 2010 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RTTI_DECLSPEC_HPP_INCLUDED
#define SWEET_RTTI_DECLSPEC_HPP_INCLUDED

#if defined(BUILD_MODULE_RTTI) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_RTTI_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_RTTI_DECLSPEC __declspec(dllimport)
#else
#define SWEET_RTTI_DECLSPEC
#endif 

#endif