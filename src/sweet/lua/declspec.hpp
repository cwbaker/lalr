//
// declspec.hpp
// Copyright (c) 2008  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_DECLSPEC_HPP
#define SWEET_LUA_DECLSPEC_HPP

#if defined(BUILD_MODULE_LUA) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_LUA_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_LUA_DECLSPEC __declspec(dllimport)
#else
#define SWEET_LUA_DECLSPEC
#endif 

#endif
