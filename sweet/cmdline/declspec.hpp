//
// declspec.hpp
// Copyright (c) 2011 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_CMDLINE_DECLSPEC_HPP_INCLUDED
#define SWEET_CMDLINE_DECLSPEC_HPP_INCLUDED

#if defined(BUILD_MODULE_CMDLINE) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_CMDLINE_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_CMDLINE_DECLSPEC __declspec(dllimport)
#else
#define SWEET_CMDLINE_DECLSPEC
#endif 

#endif