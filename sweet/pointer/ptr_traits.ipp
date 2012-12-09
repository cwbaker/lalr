//
// ptr_traits.hpp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_POINTER_PTR_TRAITS_HPP_INCLUDED
#define SWEET_POINTER_PTR_TRAITS_HPP_INCLUDED

namespace sweet
{

namespace traits
{

template <class Type>
struct traits<ptr<Type> >
{
    typedef Type base_type;
    typedef ptr<Type> value_type;
    typedef ptr<Type>& reference_type;
    typedef Type* pointer_type;
    typedef ptr<Type> parameter_type;
    typedef ptr<Type> return_value_type;
};


template <class Type>
struct traits<ptr<const Type> >
{
    typedef Type base_type;
    typedef ptr<const Type> value_type;
    typedef ptr<const Type>& reference_type;
    typedef ptr<const Type>* pointer_type;
    typedef ptr<const Type> parameter_type;
    typedef ptr<const Type> return_value_type;
};


template <class Type>
struct traits<weak_ptr<Type> >
{
    typedef Type base_type;
    typedef weak_ptr<Type> value_type;
    typedef weak_ptr<Type>& reference_type;
    typedef Type* pointer_type;
    typedef weak_ptr<Type> parameter_type;
    typedef weak_ptr<Type> return_value_type;
};


template <class Type>
struct traits<weak_ptr<const Type> >
{
    typedef Type base_type;
    typedef weak_ptr<const Type> value_type;
    typedef weak_ptr<const Type>& reference_type;
    typedef weak_ptr<const Type>* pointer_type;
    typedef weak_ptr<const Type> parameter_type;
    typedef weak_ptr<const Type> return_value_type;
};

}

}

#endif