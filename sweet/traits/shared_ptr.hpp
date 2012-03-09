//
// shared_ptr.hpp
// Copyright (c) 2009 Charles Baker.  All rights reserved.
//

#ifndef SWEET_TRAITS_TRAITS_SHARED_PTR_HPP_INCLUDED
#define SWEET_TRAITS_TRAITS_SHARED_PTR_HPP_INCLUDED

#include <boost/shared_ptr.hpp>

namespace sweet
{

namespace traits
{

template <class Type>
struct traits<boost::shared_ptr<Type>>
{
    typedef Type                     base_type;
    typedef boost::shared_ptr<Type>  value_type;
    typedef boost::shared_ptr<Type>& reference_type;
    typedef Type*                    pointer_type;
    typedef boost::shared_ptr<Type>  parameter_type;
    typedef boost::shared_ptr<Type>  return_value_type;
};


template <class Type>
struct traits<boost::shared_ptr<const Type>>
{
    typedef Type                           base_type;
    typedef boost::shared_ptr<const Type>  value_type;
    typedef boost::shared_ptr<const Type>& reference_type;
    typedef boost::shared_ptr<const Type>* pointer_type;
    typedef boost::shared_ptr<const Type>  parameter_type;
    typedef boost::shared_ptr<const Type>  return_value_type;
};


template <class Type>
struct traits<boost::weak_ptr<Type>>
{
    typedef Type                   base_type;
    typedef boost::weak_ptr<Type>  value_type;
    typedef boost::weak_ptr<Type>& reference_type;
    typedef Type*                  pointer_type;
    typedef boost::weak_ptr<Type>  parameter_type;
    typedef boost::weak_ptr<Type>  return_value_type;
};


template <class Type>
struct traits<boost::weak_ptr<const Type>>
{
    typedef Type                         base_type;
    typedef boost::weak_ptr<const Type>  value_type;
    typedef boost::weak_ptr<const Type>& reference_type;
    typedef boost::weak_ptr<const Type>* pointer_type;
    typedef boost::weak_ptr<const Type>  parameter_type;
    typedef boost::weak_ptr<const Type>  return_value_type;
};

}

}

#endif