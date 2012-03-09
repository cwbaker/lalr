//
// traits.hpp
// Copyright (c) 2008 Charles Baker.  All rights reserved.
//

#ifndef SWEET_TRAITS_TRAITS_HPP_INCLUDED
#define SWEET_TRAITS_TRAITS_HPP_INCLUDED

#include <sweet/build.hpp>

namespace sweet
{

/**
// Traits library.
*/
namespace traits
{

/**
// Extract trait information from types.
*/
template <class Type>
struct traits
{
    typedef Type        base_type;          ///< The base type.
    typedef Type        value_type;         ///< The type used to store a value of the Type.
    typedef Type&       reference_type;     ///< The type used to store a reference to a value of the Type.
    typedef Type*       pointer_type;       ///< The type used to store a pointer to a value of the Type.
    typedef const Type& parameter_type;     ///< The type used to pass a value of the Type to a function.
    typedef Type        return_value_type;  ///< The type used to return a value of the Type from a function.
};


/**
// Extract trait information from pointers to types.
*/
template <class Type>
struct traits<Type*>
{
    typedef Type   base_type;
    typedef Type*  value_type;         ///< The type used to store a value of the Type.
    typedef Type*& reference_type;     ///< The type used to store a reference to a value of the Type.
    typedef Type** pointer_type;       ///< The type used to store a pointer to a value of the Type.
    typedef Type*  parameter_type;     ///< The type used to pass a value of the Type to a function.
    typedef Type*  return_value_type;  ///< The type used to return a value of the Type from a function.
};


/**
// Extract trait information from const pointers to types.
*/
template <class Type>
struct traits<const Type*>
{
    typedef Type         base_type;
    typedef const Type*  value_type;
    typedef const Type*& reference_type;
    typedef Type* const* pointer_type;
    typedef const Type*  parameter_type;
    typedef const Type*  return_value_type;
};


/**
// Extract trait information from references to types.
*/
template <class Type>
struct traits<Type&>
{
    typedef Type        base_type;
    typedef Type        value_type;
    typedef Type&       reference_type;
    typedef Type*       pointer_type;
    typedef const Type& parameter_type;
    typedef Type&       return_value_type;
};


/**
// Extract trait information from const references to types.
*/
template <class Type>
struct traits<const Type&>
{
    typedef Type        base_type;
    typedef Type        value_type;
    typedef Type&       reference_type;
    typedef Type*       pointer_type;
    typedef const Type& parameter_type;
    typedef const Type& return_value_type;
};


/**
// The type traits for a bool.
*/
template <>
struct traits<bool>
{
    typedef bool  value_type;        ///< The type used to store a bool value.
    typedef bool& reference_type;    ///< The type used to store a reference to a bool value.
    typedef bool* pointer_type;      ///< The type used to store a pointer to a bool value.
    typedef bool  parameter_type;    ///< The type used to pass a bool value to a function.
    typedef bool  return_value_type; ///< The type used to return a bool from a function.
};


/**
// The type traits for a char.
*/
template <>
struct traits<char>
{
    typedef char  value_type;        ///< The type used to store a char value.
    typedef char& reference_type;    ///< The type used to store a reference to a char value.
    typedef char* pointer_type;      ///< The type used to store a pointer to a char value.
    typedef char  parameter_type;    ///< The type used to pass a char value to a function.
    typedef char  return_value_type; ///< The type used to return a char from a function.
};


/**
// The type traits for an int.
*/
template <>
struct traits<int>
{
    typedef int  value_type;        ///< The type used to store a int value.
    typedef int& reference_type;    ///< The type used to store a reference to a int value.
    typedef int* pointer_type;      ///< The type used to store a pointer to a int value.
    typedef int  parameter_type;    ///< The type used to pass a int value to a function.
    typedef int  return_value_type; ///< The type used to return an int from a function.
};


/**
// The type traits for a float.
*/
template <>
struct traits<float>
{
    typedef float  value_type;        ///< The type used to store a float value.
    typedef float& reference_type;    ///< The type used to store a reference to a float value.
    typedef float* pointer_type;      ///< The type used to store a pointer to a float value.
    typedef float  parameter_type;    ///< The type used to pass a float value to a function.
    typedef float  return_value_type; ///< The type used to return a float from a function.
};

/**
// The type traits for a function with no arguments.
*/
template <class R>
struct traits<R (*)()>
{
    typedef R return_type;  ///< The return type of the function.  ///< The return type of the function.
};

/**
// The type traits for a function with one argument.
*/
template <class R, class A0>
struct traits<R (*)(A0)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a function with two arguments.
*/
template <class R, class A0, class A1>
struct traits<R (*)(A0, A1)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a function with three arguments.
*/
template <class R, class A0, class A1, class A2>
struct traits<R (*)(A0, A1, A2)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a function with four arguments.
*/
template <class R, class A0, class A1, class A2, class A3>
struct traits<R (*)(A0, A1, A2, A3)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a function with five arguments.
*/
template <class R, class A0, class A1, class A2, class A3, class A4>
struct traits<R (*)(A0, A1, A2, A3, A4)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a function with six arguments.
*/
template <class R, class A0, class A1, class A2, class A3, class A4, class A5>
struct traits<R (*)(A0, A1, A2, A3, A4, A5)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a member function with no arguments.
*/
template <class R, class T>
struct traits<R (T::*)()>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a const member function with no arguments.
*/
template <class R, class T>
struct traits<R (T::*)() const>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a member function with one argument.
*/
template <class R, class T, class A0>
struct traits<R (T::*)(A0)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a const member function with one argument.
*/
template <class R, class T, class A0>
struct traits<R (T::*)(A0) const>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a member function with two arguments.
*/
template <class R, class T, class A0, class A1>
struct traits<R (T::*)(A0, A1)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a const member function with two arguments.
*/
template <class R, class T, class A0, class A1>
struct traits<R (T::*)(A0, A1) const>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a member function with three arguments.
*/
template <class R, class T, class A0, class A1, class A2>
struct traits<R (T::*)(A0, A1, A2)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a const member function with three arguments.
*/
template <class R, class T, class A0, class A1, class A2>
struct traits<R (T::*)(A0, A1, A2) const>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a member function with four arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3>
struct traits<R (T::*)(A0, A1, A2, A3)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a const member function with four arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3>
struct traits<R (T::*)(A0, A1, A2, A3) const>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a member function with five arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4>
struct traits<R (T::*)(A0, A1, A2, A3, A4)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a const member function with five arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4>
struct traits<R (T::*)(A0, A1, A2, A3, A4) const>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a member function with six arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
struct traits<R (T::*)(A0, A1, A2, A3, A4, A5)>
{
    typedef R return_type;  ///< The return type of the function.
};

/**
// The type traits for a const member function with six arguments.
*/
template <class R, class T, class A0, class A1, class A2, class A3, class A4, class A5>
struct traits<R (T::*)(A0, A1, A2, A3, A4, A5) const>
{
    typedef R return_type;  ///< The return type of the function.
};

}

}

#endif
