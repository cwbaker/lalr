//
// Lua.cpp
// Copyright (c) 2007 - 2011 Charles Baker.  All rights reserved.
//

#include "Lua.hpp"
#include "LuaAllocator.hpp"
#include "LuaFileReader.hpp"
#include "LuaMemoryReader.hpp"
#include "LuaPreprocessReader.hpp"
#include "Error.hpp"
#include "lua_/lualib.h"
#include <sweet/error/macros.hpp>

using namespace sweet;
using namespace sweet::lua;

#pragma warning( disable: 4355 )

/**
// Constructor.
*/
Lua::Lua()
: lua_state_( lua_newstate(&LuaAllocator::allocate, 0) ),
  stack_trace_enabled_( false ),
  add_parameter_helper_( lua_state_, this ),
  add_member_helper_( this )
{
//
// Register a panic function to be called when the virtual machine is put 
// into an unexpected state.
//
    SWEET_ASSERT( lua_state_ );
    lua_atpanic( lua_state_, &Lua::panic );

//
// Load the Lua standard libraries.
//
    luaL_openlibs( lua_state_ );

//
// Create the weak objects metatable and table.  The metatable is used to 
// mark the table as storing weak references.  The table stores the tables 
// that correspond to an application's C++ objects that have been weakened 
// and have their lifetime managed by the Lua virtual machine.  The table
// stored using the address of the C++ object as a key.
//
// The table uses weak keys and values so that tables may hold strong 
// references back to C++ objects (via intrusive_ptrs, shared_ptrs, ptrs, 
// etc) without the C++ objects creating cyclic references back to the 
// Lua tables (because those C++ objects indirectly hold a reference to the 
// table through their LuaObject).  The table can still be resolved from the
// LuaObject because its address is stored in the weak objects table but this
// reference isn't counted during a garbage collection sweep.
//
    {
        LuaStackGuard guard( lua_state_ );
        lua_newtable( lua_state_ );
        lua_newtable( lua_state_ );
        lua_pushstring( lua_state_, "kv" );
        lua_setfield( lua_state_, -2, "__mode" );
        lua_setmetatable( lua_state_, -2 );
        lua_setfield( lua_state_, LUA_REGISTRYINDEX, WEAK_OBJECTS_KEYWORD );
    }
}

/**
// Destructor.
*/
Lua::~Lua()
{
    lua_close( lua_state_ );
    lua_state_ = NULL;
}

/**
// Get the lua_State.
//
// @return
//  The lua_State.
*/
lua_State* Lua::get_lua_state() const
{
    return lua_state_;
}

/**
// Set whether or not stack tracing is enabled.
//
// @param stack_trace_enabled
//  True to include stack traces as part of %error messages or false to leave
//  them out.
*/
void Lua::set_stack_trace_enabled( bool stack_trace_enabled )
{
    stack_trace_enabled_ = stack_trace_enabled;
}

/**
// Is stack trace information included with %error messages?
//
// @return
//  True if stack traces are included as part of %error messages otherwise
//  false.
*/
bool Lua::is_stack_trace_enabled() const
{
    return stack_trace_enabled_;
}

/**
// Is there a global variable named \e global?
//
// @param global
//  The name of the global variable to check for the existence of.
//
// @return
//  True if there is a global variable named \e global otherwise false.
*/
bool Lua::is_value( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return !lua_isnil( lua_state_, -1 ) ? true : false;
}

/**
// Is there a boolean global variable named \e global?
//
// @param global
//  The name of the global variable to check for being a boolean.
//
// @return
//  True if \e global exists and is a boolean value otherwise false.
*/
bool Lua::is_boolean( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return lua_isboolean( lua_state_, -1 ) ? true : false;
}

/**
// Is there a numeric global variable named \e global?
//
// @param global
//  The name of the global variable to check for being a number.
//
// @return
//  True if \e global exists and is a number value otherwise false.
*/
bool Lua::is_number( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return lua_isnumber( lua_state_, -1 ) ? true : false;
}

/**
// Is there a global string variable named \e global?
//
// @param global
//  The name of the global variable to check for being a string.
//
// @return
//  True if \e global exists and is a string otherwise false.
*/
bool Lua::is_string( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return lua_isstring( lua_state_, -1 ) ? true : false;
}

/**
// Is there a global function named \e global?
//
// @param global
//  The name of the global variable to check for being a function.
//
// @return
//  True if \e global exists and is a function otherwise false.
*/
bool Lua::is_function( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return lua_isfunction( lua_state_, -1 ) ? true : false;
}

/**
// Get the boolean value of \e global.
//
// @param global
//  The name of the global to get the value of (this value must exist and must
//  be able to be converted to a boolean).
//
// @return
//  The boolean value of the global.
*/
bool Lua::boolean( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return lua_toboolean( lua_state_, -1 ) ? true : false;
}

/**
// Get the integer value of \e global.
//
// @param value
//  The name of the global to get the value of (this value must exist and 
//  must be able to be converted to a number).
//
// @return
//  The integer value of the global.
*/
int Lua::integer( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return lua_tointeger( lua_state_, -1 );
}

/**
// Get the numeric value of \e global.
//
// @param value
//  The name of the global to get the value of (this value must exist and must
//  be able to be converted to a number).
//
// @return
//  The numeric value of the value.
*/
float Lua::number( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return static_cast<float>( lua_tonumber(lua_state_, -1) );
}

/**
// Get the string value of \e global.
//
// @param global
//  The name of the global to get the value of (this value must exist and 
//  must be able to be converted to a string).
//
// @return
//  The string value of the global.
*/
std::string Lua::string( const char* global ) const
{
    SWEET_ASSERT( global );
    LuaStackGuard guard( lua_state_ );
    lua_getglobal( lua_state_, global );
    return lua_tostring( lua_state_, -1 );
}

/**
// Add variables to the global namespace.
//
// @return
//  An %AddGlobal helper object that provides a convenient syntax for setting
//  the values of global variables.
*/
AddGlobal Lua::globals()
{
    return AddGlobal( lua_state_ );
}

/**
// Create a table and return an AddMember helper object to set members of that
// table.
//
// @return
//  An %AddMember helper object that provides a convenient syntax for setting
//  values on the table.
*/
AddMember Lua::table()
{
    lua_newtable( lua_state_ );
    return AddMember( &add_member_helper_ );
}

/**
// Load a script from \e reader and execute it.
//
// @param reader
//  The function to use to read data from.
//
// @param context
//  The context to pass to the reader function.
//
// @param name
//  The name to use when reporting errors.
*/
AddParameter Lua::call( lua_Reader reader, void* context, const char* name )
{
    return add_parameter_helper_.call( reader, context, name );
}

/**
// Load a script from a file and execute it.
//
// @param filename
//  The name of the file to load the script from.
//
// @param name
//  The name to give the loaded chunk in debug information (prefix this with
//  '=' to use the name literally).
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the chunk.
*/
AddParameter Lua::call( const char* filename, const char* name )
{
    LuaFileReader reader( filename );
    return add_parameter_helper_.call( &LuaFileReader::reader, &reader, name );
}

/**
// Load a script from memory and execute it.
//
// @param first
//  The first character in the script.
//
// @param last
//  One past the last character in the script.
//
// @param name
//  The name to give the imported chunk in debug information (prefix this 
//  with '=' to use the name literally).
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the chunk.
*/
AddParameter Lua::call( const char* first, const char* last, const char* name )
{
    LuaMemoryReader reader( first, last );
    return add_parameter_helper_.call( &LuaMemoryReader::reader, &reader, name );
}

/**
// Call a global function.
//
// @param function
//  The name of the function to call.
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the function.
*/
AddParameter Lua::call( const char* function )
{
    SWEET_ASSERT( function );
    return add_parameter_helper_.call( function );
}

/**
// Call a function in the main thread of this %Lua virtual machine.
//
// @param value
//  A LuaValue that refers to the function to call.
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the function.
*/
AddParameter Lua::call( const LuaValue& value )
{
    return add_parameter_helper_.call( value );
}

/**
// Preprocess a script from a file and execute it.
//
// The preprocess step replace all occurences of \e prefix with 
// \e prefix_replacement and all occurences of \e suffix with 
// \e suffix_replacement and escapes any occurences of \e prefix_replacement
// that occur between \e suffix and \e prefix by its octal escaped literal.
//
// The following example allows a file to be loaded in which %Lua is used as a 
// template processing language similar to PHP or ASP.
//
@code

Lua lua;
lua.preprocess( "cxx.luat", "cxx" );

@endcode
//
// @param filename
//  The name of the file to load the script from.
//
// @param name
//  The name to give the loaded chunk in debug information (prefix this 
//  with '=' to use the name literally).
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the chunk.
*/
AddParameter Lua::preprocess( const char* filename, const char* name )
{
    LuaFileReader file_reader( filename );
    LuaPreprocessReader preprocess_reader( &LuaFileReader::reader, &file_reader );
    return add_parameter_helper_.call( &LuaPreprocessReader::reader, &preprocess_reader, name );
}

/**
// Preprocess a script from memory and execute it.
//
// @param first
//  The first character in the script.
//
// @param last
//  One past the last character in the script.
//
// @param name
//  The name to give the imported chunk in debug information (prefix this 
//  with '=' to use the name literally).
//
// @return
//  An %AddParameter helper that provides a convenient syntax for pushing
//  parameters to, calling, and retrieving return values from the chunk.
*/
AddParameter Lua::preprocess( const char* first, const char* last, const char* name )
{
    LuaMemoryReader memory_reader( first, last );
    LuaPreprocessReader preprocess_reader( &LuaMemoryReader::reader, &memory_reader );
    return add_parameter_helper_.call( &LuaPreprocessReader::reader, &preprocess_reader, name );
}

/**
// Handle panic errors.
//
// @param lua_state
//  The lua_State that is in a panic.
// 
// @return
//  Always 0 (but never actually returns because it throws an exception).
*/
int Lua::panic( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
    SWEET_ERROR( PanicError("Panic - %s", lua_tostring(lua_state, -1)) );
    return 0;
}
