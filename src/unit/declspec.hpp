//
// declspec.hpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#ifndef UNITTEST_DECLSPEC_HPP
#define UNITTEST_DECLSPEC_HPP

#if defined(BUILD_MODULE_LIBUNITTEST) && defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define UNITTEST_DECLSPEC __declspec(dllexport)
#elif defined(BUILD_LIBRARY_TYPE_DYNAMIC)
#define UNITTEST_DECLSPEC __declspec(dllimport)
#else
#define UNITTEST_DECLSPEC
#endif 

#endif
