//
// Type.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RTTI_TYPE_HPP_INCLUDED
#define SWEET_RTTI_TYPE_HPP_INCLUDED

#include "declspec.hpp"
#include "macros.hpp"

namespace sweet
{

namespace rtti
{

/**
// A lightweight wrapper around type_info that allows it to be replaced by 
// user implementations.
*/
class SWEET_RTTI_DECLSPEC Type
{
    const TypeInfo* type_info_;

    public:
        Type( const TypeInfo& type_info );
        Type( const Type& type );
        Type& operator=( const Type& type );

        const char* name() const;
        bool operator==( const Type& type ) const;
        bool operator!=( const Type& type ) const;
        bool operator<( const Type& type ) const;
};

}

}

#endif
