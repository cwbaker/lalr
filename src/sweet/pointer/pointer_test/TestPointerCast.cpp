//
// TestPointerCast.cpp
// Copyright (c) Charles Baker. All rights reserved.
//

#include <unit/UnitTest.h>
#include <sweet/pointer/ptr.hpp>

using namespace sweet::pointer;

class Object
{
};

class DerivedObject : public Object
{
};

SUITE( TestPointerCast )
{
    TEST( StaticCast )
    {
        ptr<DerivedObject> derived_object( new DerivedObject() );
        ptr<Object> object = static_pointer_cast<Object>( derived_object );
        CHECK( object.get() );
        CHECK( object.get() == derived_object.get() );
    }

    TEST( ConstCast )
    {
        ptr<const Object> const_object( new Object() );
        ptr<Object> object = const_pointer_cast<Object>( const_object );
        CHECK( object.get() );
        CHECK( object.get() == const_object.get() );
    }

    TEST( DynamicCast )
    {
        ptr<DerivedObject> derived_object( new DerivedObject() );
        ptr<Object> object = dynamic_pointer_cast<Object>( derived_object );
        CHECK( object.get() );
        CHECK( object.get() == derived_object.get() );
    }
}
