//
// error_macros.hpp
// Copyright (c) 2001 - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_ERROR_ERROR_MACROS_HPP_INCLUDED
#define SWEET_ERROR_ERROR_MACROS_HPP_INCLUDED

#include <sweet/build.hpp>

#ifdef SWEET_EXCEPTIONS_ENABLED
#define SWEET_ERROR( e ) throw e;
#else
#define SWEET_ERROR( e ) sweet::error::error( e );
#endif

#endif
