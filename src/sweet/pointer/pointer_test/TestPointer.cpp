//
// TestPointer.cpp
// Copyright (c) 2009 - 2012 Charles Baker.  All rights reserved.
//

#include <sweet/unit/UnitTest.h>
#include <sweet/pointer/ptr.hpp>

using namespace sweet::pointer;

class Object
{
    bool* destroyed_;

    public:
        Object( bool* destroyed )
        : destroyed_( destroyed )
        {
            SWEET_ASSERT( destroyed_ );
            *destroyed_ = false;
        }

        ~Object()
        {
            *destroyed_ = true;
        }

        bool valid() const
        {
            return !*destroyed_;
        }
};

SUITE( TestPointer )
{
    TEST( TestPtr )
    {
        bool destroyed = false;

        ptr<Object> object( new Object(&destroyed) );
        CHECK( !destroyed );

        ptr<Object> copied_object( object );
        CHECK( !destroyed );

        ptr<Object> assigned_object;
        assigned_object = object;
        CHECK( object == assigned_object );
        CHECK( !destroyed );

        weak_ptr<Object> weak_object( object );
        CHECK( weak_object.lock() == object );
        CHECK( !destroyed );

        CHECK( object->valid() );
        CHECK( (*object).valid() );
        CHECK( object.get() != NULL );
        CHECK( object.get()->valid() );

        ptr<Object> swapped_object;
        swapped_object.swap( object );
        CHECK( swapped_object = copied_object );        
        CHECK( swapped_object = assigned_object );
        CHECK( !destroyed );
        CHECK( object.get() == NULL );

        copied_object.reset();
        assigned_object.reset();
        swapped_object.reset();
        CHECK( destroyed );
    }

    TEST( TestWeakPtr )
    {
        bool destroyed = false;
        ptr<Object> object( new Object(&destroyed) );
        CHECK( !destroyed );

        weak_ptr<Object> weak_object( object );
        CHECK( !destroyed );

        weak_ptr<Object> assigned_weak_object;
        assigned_weak_object = object;
        CHECK( !destroyed );
        CHECK( assigned_weak_object == weak_object );

        weak_ptr<Object> other_weak_object( weak_object );
        CHECK( !destroyed );
        CHECK( other_weak_object == weak_object );

        weak_ptr<Object> other_assigned_weak_object;
        other_assigned_weak_object = weak_object;
        CHECK( !destroyed );
        CHECK( other_assigned_weak_object == weak_object );
        
        ptr<Object> promoted_object = weak_object.lock();
        CHECK( !destroyed );
        CHECK( promoted_object == object );

        weak_ptr<Object> swapped_weak_object;
        swapped_weak_object.swap( weak_object );
        CHECK( !destroyed );
        CHECK( swapped_weak_object == assigned_weak_object );
        CHECK( swapped_weak_object == other_weak_object );
        CHECK( swapped_weak_object == other_assigned_weak_object );
        CHECK( weak_object.lock().get() == NULL );

        object.reset();
        promoted_object.reset();        
        CHECK( destroyed );

        promoted_object = weak_object.lock();
        CHECK( promoted_object.get() == NULL );

        weak_object.reset();
        assigned_weak_object.reset();
        other_weak_object.reset();
        other_assigned_weak_object.reset();
        swapped_weak_object.reset();
    }
}
