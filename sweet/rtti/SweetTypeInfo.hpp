//
// UserDefinedTypeInfo.hpp
// Copyright (c) 2008 - 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RTTI_SWEETTYPEINFO_HPP_INCLUDED
#define SWEET_RTTI_SWEETTYPEINFO_HPP_INCLUDED

#include "declspec.hpp"

namespace sweet
{

namespace rtti
{

/**
// User defined type information.
*/
class SWEET_RTTI_DECLSPEC SweetTypeInfo
{
    const char* name_; ///< The name of the type.

    public:
        SweetTypeInfo( const char* name );

        const char* name() const;
        const char* raw_name() const;
        bool before( const SweetTypeInfo& type_info ) const;

        bool operator==( const SweetTypeInfo& type_info ) const;
        bool operator!=( const SweetTypeInfo& type_info ) const;
};

}

}

#endif
