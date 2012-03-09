//
// declspec.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef SWEET_PARSER_DECLSPEC_HPP_INCLUDED
#define SWEET_PARSER_DECLSPEC_HPP_INCLUDED

#include <sweet/build.hpp>

#if defined(BUILD_MODULE_PARSER) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_PARSER_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define SWEET_PARSER_DECLSPEC __declspec(dllimport)
#else
#define SWEET_PARSER_DECLSPEC
#endif 

#endif
