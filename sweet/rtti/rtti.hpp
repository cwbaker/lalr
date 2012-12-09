//
// rtti.hpp
// Copyright (c) 2007 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RTTI_RTTI_HPP_INCLUDED
#define SWEET_RTTI_RTTI_HPP_INCLUDED

#include <sweet/build.hpp>
#include "macros.hpp"
#include "Type.hpp"

#ifndef BUILD_MODULE_RTTI
#pragma comment( lib, "rtti" BUILD_LIBRARY_SUFFIX )
#endif

namespace sweet
{

/**
 Run time type information library.

 The rtti component provides a light wrapper around the default run time type 
 information provided by the compiler and runtime so that applications can 
 provide their own implementation if necessary while still providing the 
 libraries that use it with a consistent interface.

 There are three modes of operation available.  The mode of operation is selected
 at compile time based on the macros SWEET_RTTI_ENABLED, 
 SWEET_RTTI_SWEET_RTTI_ENABLED, and SWEET_RTTI_USER_RTTI_ENABLED.

 The interface provided is a class, TypeInfo, that is used to represent type 
 information and two macros SWEET_TYPEID() and SWEET_STATIC_TYPEID() that are
 used to convert classes or types to their equivalent TypeInfos are runtime
 and compile time respectively.

 If the macro SWEET_RTTI_ENABLED is defined then compiler supported runtime 
 type information is selected.  The compiler defined type_info is typedef'd 
 to TypeInfo and the SWEET_TYPEID() and SWEET_STATIC_TYPEID() macros are 
 both defined to evaluate to typeid().

 If the macro SWEET_RTTI_SWEET_RTTI_ENABLED is defined then the Sweet custom 
 runtime type information is selected.  The SweetTypeInfo class is typedef'd 
 to TypeInfo and the SWEET_TYPEID() macro is defined to evaluate to 
 sweet_typeid() while the SWEET_STATIC_TYPEID() macro evaluates to 
 x::sweet_static_typeid().

 If the macro SWEET_RTTI_USER_RTTI_ENABLED is defined then user specified 
 custom runtime type information is selected.  The user must define the macros
 SWEET_RTTI_USER_TYPE_INFO, SWEET_RTTI_USER_TYPEID(), and SWEET_RTTI_USER_STATIC_TYPEID() 
 to evaluate to the name of the user's TypeInfo equivalent type (that must implement the 
 same interface as type_info), the user's typeid equivalent function, and the user's
 static typeid equivalent function respectively.

*/
namespace rtti
{
};

};

#endif  // #ifndef SWEET_RTTI_HPP_INCLUDED
