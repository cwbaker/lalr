//
// macros.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RTTI_MACROS_HPP_INCLUDED
#define SWEET_RTTI_MACROS_HPP_INCLUDED

#include <sweet/build.hpp>

//
// If SWEET_RTTI_ENABLED is defined then compiler supported runtime type
// information is being used.  The compiler defined type_info is typedef'd to
// be TypeInfo and the SWEET_TYPEID() and SWEET_STATIC_TYPEID() macros are 
// defined to evaluate to typeid().
//
#if defined(SWEET_RTTI_ENABLED)

#include <typeinfo>

namespace sweet
{

namespace rtti
{

typedef std::type_info TypeInfo;
#define SWEET_TYPEID(x) typeid(x)
#define SWEET_STATIC_TYPEID(x) typeid(x)

}

}

//
// If SWEET_RTTI_SWEET_RTTI_ENABLED is defined then the Sweet custom runtime
// type information is being used.  The SweetTypeInfo class is typedef'd to
// be TypeInfo and the SWEET_TYPEID() macro is defined to evaluate to 
// sweet_typeid() and the SWEET_STATIC_TYPEID() evaluates to 
// x::sweet_static_typeid().
//
#elif defined(SWEET_RTTI_SWEET_RTTI_ENABLED)

#include "SweetTypeInfo.hpp"

namespace sweet
{

namespace rtti
{

typedef SweetTypeInfo TypeInfo;
#define SWEET_TYPEID(x) sweet_typeid(x)
#define SWEET_STATIC_TYPEID(x) (x::sweet_static_typeid())

}

}

//
// If SWEET_RTTI_USER_RTTI_ENABLED is defined then user specified custom
// runtime type information is being used.  The user must provide the macros
// SWEET_RTTI_USER_TYPE_INFO that evaluates to give the name of the user's 
// TypeInfo equivalent type (that must implement the same interface as 
// type_info) and SWEET_RTTI_USER_TYPEID() that evalues to give the user's
// typeid equivalent function.
//
#elif defined(SWEET_RTTI_USER_RTTI_ENABLED)

#ifndef SWEET_RTTI_USER_TYPE_INFO
#error "When providing user type information by defining SWEET_RTTI_USER_RTTI_ENABLED you must provide the macro SWEET_RTTI_USER_TYPE_INFO to specify your type_info type."
#endif

#ifndef SWEET_RTTI_USER_TYPEID
#error "When providing user type information by defining SWEET_RTTI_USER_RTTI_ENABLED you must provide the macro SWEET_RTTI_USER_TYPEID() to specify your typeid function."
#endif

#ifndef SWEET_RTTI_USER_STATIC_TYPEID
#error "When providing user type information by defining SWEET_RTTI_USER_RTTI_ENABLED you must provide the macro SWEET_RTTI_USER_STATIC_TYPEID() to specify your static typeid function."
#endif

namespace sweet
{

namespace rtti
{

typedef SWEET_RTTI_USER_TYPE_INFO TypeInfo;
#define SWEET_TYPEID(x) SWEET_RTTI_USER_TYPEID(x)
#define SWEET_STATIC_TYPEID(x) SWEET_RTTI_USER_STATIC_TYPEID(x)

}

}

#else

#error "One of SWEET_RTTI_ENABLED, SWEET_RTTI_SWEET_RTTI_ENABLED, or SWEET_RTTI_USER_RTTI_ENABLED must be defined."

#endif

#endif
