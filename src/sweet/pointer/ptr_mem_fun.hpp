//
// ptr_mem_fun.hpp
// Copyright (c) 2014 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_PTR_MEM_FUN_HPP_INCLUDED
#define SWEET_POINTER_PTR_MEM_FUN_HPP_INCLUDED

namespace sweet
{

namespace pointer
{

template <class Type> class ptr;

/**
// Bind a member function call to a function object.
*/
template <class R, class T>
class ptr_mem_fun_t
{
    typedef R (T::* MemberFunction)();
    MemberFunction function_;

    public:
        ptr_mem_fun_t( const MemberFunction& function )
        : function_( function )
        {
        }

        bool operator()( const ptr<T>& object ) const
        {
            return (object.get()->*function_)();
        }
};

/**
// Bind a const member function call to a function object.
*/
template <class R, class T>
class ptr_const_mem_fun_t
{
    typedef R (T::* ConstMemberFunction)() const;
    ConstMemberFunction function_;

    public:
        ptr_const_mem_fun_t( const ConstMemberFunction& function )
        : function_( function )
        {
        }

        bool operator()( const ptr<T>& object ) const
        {
            return (object.get()->*function_)();
        }
};

template <class R, class T> ptr_mem_fun_t<R, T> ptr_mem_fun( R (T::* function)() )
{
    return ptr_mem_fun_t<R, T>( function );
}

template <class R, class T> ptr_const_mem_fun_t<R, T> ptr_const_mem_fun( R (T::* function)() const )
{
    return ptr_const_mem_fun_t<R, T>( function );
}

}

using pointer::ptr_mem_fun;
using pointer::ptr_const_mem_fun;

}

#endif
