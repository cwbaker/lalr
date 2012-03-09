//
// Type.cpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#include "Type.hpp"
#include <sweet/assert/assert.hpp>

using namespace sweet::rtti;

/**
// Constructor.
//
// @param TypeInfo
//  The type_info to wrap.
*/
Type::Type( const TypeInfo& type_info )
: type_info_( &type_info )
{
    SWEET_ASSERT( type_info_ );
}

/**
// Copy constructor.
//
// @param type
//  The Type to copy.
*/
Type::Type( const Type& type )
: type_info_( type.type_info_ )
{
    SWEET_ASSERT( type_info_ );
}

/**
// Assignment operator.
//
// @param type
//  The Type to assign from.
//
// @return
//  This Type.
*/
Type& Type::operator=( const Type& type )
{
    if ( this != &type )
    {
        type_info_ = type.type_info_;
        SWEET_ASSERT( type_info_ );
    }
    return *this;
}

/**
// Get the name of this Type.
//
// @return
//  The name.
*/
const char* Type::name() const
{
    return type_info_->name();
}

/**
// Equality operator.
//
// @param type
//  The Type to compare with.
//
// @return
//  True if this Type and \e type are equal otherwise false.
*/
bool Type::operator==( const Type& type ) const
{
    return *type_info_ == *type.type_info_;
}

/**
// Not equal operator.
//
// @param type
//  The Type to compare with.
//
// @return
//  True if this Type and \e type are not equal otherwise false.
*/
bool Type::operator!=( const Type& type ) const
{
    return *type_info_ != *type.type_info_;
}

/**
// Less than operator.
//
// @param type
//  The Type to compare with.
//
// @return
//  True if this Type is less than \e type otherwise false.
*/
bool Type::operator<( const Type& type ) const
{
    return type_info_->before( *type.type_info_ );
}
