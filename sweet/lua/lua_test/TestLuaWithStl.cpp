//
// TestLuaWithStl.cpp
// Copyright (c) 2008 - 2010 Charles Baker.  All rights reserved.
//

#include <sweet/unit/UnitTest.h>
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/vector.hpp>
#include <sweet/lua/list.hpp>
#include <sweet/lua/set.hpp>
#include <sweet/lua/map.hpp>

using std::make_pair;
using namespace sweet::lua;

SUITE( TestLuaWithStl )
{
    struct Fixture
    {
        Lua lua_;
        std::vector<int> int_vector_;
        std::list<int> int_list_;
        std::set<int> int_set_;
        std::multiset<int> int_multiset_;
        std::map<int, int> int_map_;
        std::multimap<int, int> int_multimap_;

        const std::vector<int>& int_vector() const
        {
            return int_vector_;
        }

        const std::list<int>& int_list() const
        {
            return int_list_;
        }

        const std::set<int>& int_set() const
        {
            return int_set_;
        }

        const std::multiset<int>& int_multiset() const
        {
            return int_multiset_;
        }

        const std::map<int, int>& int_map() const
        {
            return int_map_;
        }

        const std::multimap<int, int>& int_multimap() const
        {
            return int_multimap_;
        }
    };

    TEST_FIXTURE( Fixture, TestVector )
    {
        int_vector_.push_back( 1 );
        int_vector_.push_back( 2 );
        int_vector_.push_back( 3 );
        int_vector_.push_back( 4 );
        lua_.globals()( "integers", &Fixture::int_vector, this );
        const char script[] = "local sum = 0; for i in integers() do sum = sum + i end; return sum;";
        int sum = 0;
        lua_.call( script, script + sizeof(script) - 1, "TestVector" ).end( &sum );
        CHECK( sum == 1 + 2 + 3 + 4 );
    }

    TEST_FIXTURE( Fixture, TestList )
    {
        int_list_.push_back( 1 );
        int_list_.push_back( 2 );
        int_list_.push_back( 3 );
        int_list_.push_back( 4 );
        lua_.globals()( "integers", &Fixture::int_list, this );
        const char script[] = "local sum = 0; for i in integers() do sum = sum + i end; return sum;";
        int sum = 0;
        lua_.call( script, script + sizeof(script) - 1, "TestList" ).end( &sum );
        CHECK( sum == 1 + 2 + 3 + 4 );
    }

    TEST_FIXTURE( Fixture, TestSet )
    {
        int_set_.insert( 1 );
        int_set_.insert( 2 );
        int_set_.insert( 3 );
        int_set_.insert( 4 );
        lua_.globals()( "integers", &Fixture::int_set, this );
        const char script[] = "local sum = 0; for i in integers() do sum = sum + i end; return sum;";
        int sum = 0;
        lua_.call( script, script + sizeof(script) - 1, "TestSet" ).end( &sum );
        CHECK( sum == 1 + 2 + 3 + 4 );
    }

    TEST_FIXTURE( Fixture, TestMultiset )
    {
        int_multiset_.insert( 1 );
        int_multiset_.insert( 2 );
        int_multiset_.insert( 3 );
        int_multiset_.insert( 4 );
        lua_.globals()( "integers", &Fixture::int_multiset, this );
        const char script[] = "local sum = 0; for i in integers() do sum = sum + i end; return sum;";
        int sum = 0;
        lua_.call( script, script + sizeof(script) - 1, "TestMultiset" ).end( &sum );
        CHECK( sum == 1 + 2 + 3 + 4 );
    }

    TEST_FIXTURE( Fixture, TestMap )
    {
        int_map_.insert( make_pair(1, 1) );
        int_map_.insert( make_pair(2, 2) );
        int_map_.insert( make_pair(3, 3) );
        int_map_.insert( make_pair(4, 4) );
        lua_.globals()( "integers", &Fixture::int_map, this );
        const char script[] = "for i in integers() do end";
        lua_.call( script, script + sizeof(script) - 1, "TestMap" ).end();
    }

    TEST_FIXTURE( Fixture, TestMultimap )
    {
        int_multimap_.insert( make_pair(1, 1) );
        int_multimap_.insert( make_pair(2, 2) );
        int_multimap_.insert( make_pair(3, 3) );
        int_multimap_.insert( make_pair(4, 4) );
        lua_.globals()( "integers", &Fixture::int_multimap, this );
        const char script[] = "for i in integers() do end";
        lua_.call( script, script + sizeof(script) - 1, "TestMultimap" ).end();
    }
}