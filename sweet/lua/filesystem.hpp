//
// filesystem.hpp
// Copyright (c) 2007  - 2010 Charles Baker.  All rights reserved.
//

#ifndef SWEET_LUA_FILESYSTEM_HPP_INCLUDED
#define SWEET_LUA_FILESYSTEM_HPP_INCLUDED

#include <boost/filesystem.hpp>

namespace sweet
{

namespace lua
{

/**
// @internal
//
// Convert boost::filesystem::basic_directory_entries that are pushed onto
// the Lua stack into strings.
//
// @todo
//  Pushing a boost::filesystem::basic_directory_entry is currently templated
//  on the type of path (allowing different character types) even though only 
//  narrow character strings have been considered.
*/
template <class Path>
struct LuaConverter<boost::filesystem::basic_directory_entry<Path>>
{
    /**
    // @internal
    //
    // @param lua
    //  The lua_State to push the string onto the stack of.
    //
    // @param value
    //  The boost::filesystem::basic_directory_entry to push.
    */
    static void push( lua_State* lua_state, const boost::filesystem::basic_directory_entry<Path>& entry  )
    {
        SWEET_ASSERT( lua_state != NULL );
        lua_pushlstring( lua_state, entry.path().string().c_str(), entry.path().string().length() );
    }
};



/**
// @internal
//
// Convert boost::filesystem::basic_directory_iterators that are pushed onto 
// the Lua stack into Lua iterator functions.
//
// This pushes an iterator that will iterator from the 
// basic_directory_iterator specified by \e value to the default constructed
// basic_directory_iterator.
//
// @todo
//  Pushing a boost::filesystem::basic_directory_iterator is currently 
//  templated on the type of path (allowing different character types) even 
//  though only narrow character strings have been considered.
//
// @param lua
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The boost::filesystem::basic_directory_iterator to begin the iteration at.
*/
template <class Path>
struct LuaConverter<boost::filesystem::basic_directory_iterator<Path>>
{
    static void push( lua_State* lua_state, const boost::filesystem::basic_directory_iterator<Path>& value )
    {
        lua_push_iterator( lua_state, value, boost::filesystem::basic_directory_iterator<Path>() );
    }

    static const boost::filesystem::basic_directory_iterator<Path>& to( lua_State* lua_state, int position )
    {
        return lua_to_value<boost::filesystem::basic_directory_iterator<Path>>( lua_state, position );
    }
};


/**
// @internal
//
// Convert boost::filesystem::basic_recursive_directory_iterators that are 
// pushed onto the Lua stack into Lua iterator functions.
//
// This pushes an iterator that will iterate from the 
// basic_recursive_directory_iterator specified by \e value to the default 
// constructed basic_recursive_directory_iterator that marks the end of the
// iteration.
//
// @todo
//  Pushing a boost::filesystem::basic_recursive_directory_iterator is 
//  currently templated on the type of path (allowing different character 
//  types) even though only narrow character strings have been considered.
//
// @param lua_state
//  The lua_State to push the iterator function onto the stack of.
//
// @param value
//  The boost::filesystem::basic_directory_iterator to begin the iteration at.
*/
template <class Path>
struct LuaConverter<boost::filesystem::basic_recursive_directory_iterator<Path>>
{
    static void push( lua_State* lua_state, const boost::filesystem::basic_recursive_directory_iterator<Path>& value )
    {
        lua_push_iterator( lua_state, value, boost::filesystem::basic_recursive_directory_iterator<Path>() );
    }

    static const boost::filesystem::basic_recursive_directory_iterator<Path>& to( lua_State* lua_state, int position )
    {
        return lua_to_value<boost::filesystem::basic_recursive_directory_iterator<Path>>( lua_state, position );
    }
};

}

}

#endif
