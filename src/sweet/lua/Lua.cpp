//
// Lua.cpp
// Copyright (c) Charles Baker.  All rights reserved.
//

#include "Lua.hpp"
#include "LuaValue.hpp"
#include "LuaAllocator.hpp"
#include "LuaFileReader.hpp"
#include "LuaMemoryReader.hpp"
#include "LuaPreprocessReader.hpp"
#include <lua/lua.hpp>
#include <sweet/error/macros.hpp>
#include <sweet/build.hpp>
#include <stdexcept>

#ifdef SWEET_LUA_MOBDEBUG_ENABLED
#include <luasocket/luasocket.hpp>
#endif

using namespace sweet;
using namespace sweet::lua;

#ifdef _MSC_VER
#pragma warning( disable: 4355 )
#endif

/**
// Constructor.
*/
Lua::Lua( error::ErrorPolicy& error_policy )
: error_policy_( error_policy ),
  lua_state_( lua_newstate(&LuaAllocator::allocate, 0) ),
  mobdebug_( NULL ),
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
    destroy();
}

/**
// Destroy the Lua virtual machine for this Lua object.
//
// This destroys the Lua virtual machine for this Lua object but leaves the
// Lua object itself partially valid.  It is still possible to make 
// template <class Type> Lua::destroy( Type& object ) calls which will quietly
// do nothing.  This allows any references to objects held by the Lua virtual 
// machine to be released immediately (as the virtual machine is destroyed) 
// while still allowing other parts of a larger application to make calls to
// destroy their associated Lua objects without causing any problems.
*/
void Lua::destroy()
{
    if ( mobdebug_ )
    {
        delete mobdebug_;
        mobdebug_ = NULL;
    }

    if ( lua_state_ )
    {
        lua_close( lua_state_ );
        lua_state_ = NULL;
    }
}

/**
// Get the ErrorPolicy that this Lua object reports errors to.
//
// @return
//  The ErrorPolicy.
*/
error::ErrorPolicy& Lua::error_policy() const
{
    return error_policy_;
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
// Get the LuaValue corresponding to the "mobdebug" table.
//
// @return
//  The LuaValue corresponding to the "mobdebug" table or null if 
//  Lua::mobdebug_start() has not been called.
*/
LuaValue* Lua::mobdebug() const
{
    return mobdebug_;
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
// Start Zero Brane Studio's mobdebug service.
//
// This function silently does nothing unless the macro 
// SWEET_LUA_MOBDEBUG_ENABLED is defined at compile time.
//
// @param mobdebug
//  The full path to "mobdebug.lua" or null to load "mobdebug.lua" from the 
//  current working directory of the process (assumed not null).
*/
void Lua::mobdebug_start( const char* mobdebug )
{
#ifdef SWEET_LUA_MOBDEBUG_ENABLED
    if ( !mobdebug_ && mobdebug )
    {
        error_policy_.push_errors();
        
        lua_getglobal( lua_state_, "package" );
        lua_getfield( lua_state_, -1, "preload" );
        lua_pushcfunction( lua_state_, &luaopen_socket_core );
        lua_setfield( lua_state_, -2, "socket.core" );
        lua_pushcfunction( lua_state_, &Lua::open_socket );
        lua_setfield( lua_state_, -2, "socket" );
        lua_pop( lua_state_, 2 );

        LuaValue value;
        call( mobdebug, "mobdebug.lua" ).end( &value );
        error_policy_.error( !is_table(value), "Loading mobdebug from '%s' doesn't return a table", mobdebug );
        int errors = error_policy_.pop_errors();
        if ( errors == 0 )
        {
            call( "start", value ).end();
            mobdebug_ = new LuaValue( value );
        }
    }
#else
    (void) mobdebug;
#endif
}

/**
// Turn on Zero Brane Studio debugging for the main Lua coroutine.
//
// This function silently does nothing unless the macro 
// SWEET_LUA_MOBDEBUG_ENABLED is defined at compile time or if 
// Lua::mobdebug_start() has not already been called at runtime.
*/
void Lua::mobdebug_on()
{
#ifdef SWEET_LUA_MOBDEBUG_ENABLED
    if ( mobdebug_ )
    {
        call( "on", *mobdebug_ ).end();
    }
#endif
}

/**
// Turn off Zero Brane Studio debugging for the main Lua coroutine.
//
// This function silently does nothing unless the macro 
// SWEET_LUA_MOBDEBUG_ENABLED is defined at compile time or if 
// Lua::mobdebug_start() has not already been called at runtime.
*/
void Lua::mobdebug_off()
{
#ifdef SWEET_LUA_MOBDEBUG_ENABLED
    if ( mobdebug_ )
    {
        call( "off", *mobdebug_ ).end();
    }
#endif
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
// Is there a function associated with the LuaValue \e value?
//
// @param value
//  The LuaValue object that represents a Lua value to check for being a 
//  function.
//
// @return
//  True if the Lua value of \e value is a function otherwise false.
*/
bool Lua::is_function( const LuaValue& value ) const
{
    LuaStackGuard guard( lua_state_ );
    lua_push( lua_state_, value );
    return lua_isfunction( lua_state_, -1 ) ? true : false;
}

/**
// Is there a table associated with the LuaValue \e value?
//
// @param value
//  The LuaValue object that represents a Lua value to check for being a
//  table.
//
// @return 
//  True if the Lua value of \e value is a table otherwise false.
*/
bool Lua::is_table( const LuaValue& value ) const
{
    LuaStackGuard guard( lua_state_ );
    lua_push( lua_state_, value );
    return lua_istable( lua_state_, -1 ) ? true : false;
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
    return int( luaL_checkinteger(lua_state_, -1) );
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
    LuaFileReader reader( filename, error_policy_ );
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
    LuaFileReader file_reader( filename, error_policy_ );
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
    SWEET_ERROR( std::runtime_error(lua_tostring(lua_state, -1)) );
    return 0;
}

/**
// Entry-point function for 'socket.lua' set to preload["socket"].
*/
int Lua::open_socket( lua_State* lua_state )
{
    SWEET_ASSERT( lua_state );
#if defined SWEET_LUA_MOBDEBUG_ENABLED
    const char* SOCKET_LUA =
        "-----------------------------------------------------------------------------\n"
        "-- LuaSocket helper module\n"
        "-- Author: Diego Nehab\n"
        "-----------------------------------------------------------------------------\n"
        "\n"
        "-----------------------------------------------------------------------------\n"
        "-- Declare module and import dependencies\n"
        "-----------------------------------------------------------------------------\n"
        "local base = _G\n"
        "local string = require(\"string\")\n"
        "local math = require(\"math\")\n"
        "local socket = require(\"socket.core\")\n"
        "\n"
        "local _M = socket\n"
        "\n"
        "-----------------------------------------------------------------------------\n"
        "-- Exported auxiliar functions\n"
        "-----------------------------------------------------------------------------\n"
        "function _M.connect4(address, port, laddress, lport)\n"
        "    return socket.connect(address, port, laddress, lport, \"inet\")\n"
        "end\n"
        "\n"
        "function _M.connect6(address, port, laddress, lport)\n"
        "    return socket.connect(address, port, laddress, lport, \"inet6\")\n"
        "end\n"
        "\n"
        "function _M.bind(host, port, backlog)\n"
        "    if host == \"*\" then host = \"0.0.0.0\" end\n"
        "    local addrinfo, err = socket.dns.getaddrinfo(host);\n"
        "    if not addrinfo then return nil, err end\n"
        "    local sock, res\n"
        "    err = \"no info on address\"\n"
        "    for i, alt in base.ipairs(addrinfo) do\n"
        "        if alt.family == \"inet\" then\n"
        "            sock, err = socket.tcp4()\n"
        "        else\n"
        "            sock, err = socket.tcp6()\n"
        "        end\n"
        "        if not sock then return nil, err end\n"
        "        sock:setoption(\"reuseaddr\", true)\n"
        "        res, err = sock:bind(alt.addr, port)\n"
        "        if not res then\n"
        "            sock:close()\n"
        "        else\n"
        "            res, err = sock:listen(backlog)\n"
        "            if not res then\n"
        "                sock:close()\n"
        "            else\n"
        "                return sock\n"
        "            end\n"
        "        end\n"
        "    end\n"
        "    return nil, err\n"
        "end\n"
        "\n"
        "_M.try = _M.newtry()\n"
        "\n"
        "function _M.choose(table)\n"
        "    return function(name, opt1, opt2)\n"
        "        if base.type(name) ~= \"string\" then\n"
        "            name, opt1, opt2 = \"default\", name, opt1\n"
        "        end\n"
        "        local f = table[name or \"nil\"]\n"
        "        if not f then base.error(\"unknown key (\".. base.tostring(name) ..\")\", 3)\n"
        "        else return f(opt1, opt2) end\n"
        "    end\n"
        "end\n"
        "\n"
        "-----------------------------------------------------------------------------\n"
        "-- Socket sources and sinks, conforming to LTN12\n"
        "-----------------------------------------------------------------------------\n"
        "-- create namespaces inside LuaSocket namespace\n"
        "local sourcet, sinkt = {}, {}\n"
        "_M.sourcet = sourcet\n"
        "_M.sinkt = sinkt\n"
        "\n"
        "_M.BLOCKSIZE = 2048\n"
        "\n"
        "sinkt[\"close-when-done\"] = function(sock)\n"
        "    return base.setmetatable({\n"
        "        getfd = function() return sock:getfd() end,\n"
        "        dirty = function() return sock:dirty() end\n"
        "    }, {\n"
        "        __call = function(self, chunk, err)\n"
        "            if not chunk then\n"
        "                sock:close()\n"
        "                return 1\n"
        "            else return sock:send(chunk) end\n"
        "        end\n"
        "    })\n"
        "end\n"
        "\n"
        "sinkt[\"keep-open\"] = function(sock)\n"
        "    return base.setmetatable({\n"
        "        getfd = function() return sock:getfd() end,\n"
        "        dirty = function() return sock:dirty() end\n"
        "    }, {\n"
        "        __call = function(self, chunk, err)\n"
        "            if chunk then return sock:send(chunk)\n"
        "            else return 1 end\n"
        "        end\n"
        "    })\n"
        "end\n"
        "\n"
        "sinkt[\"default\"] = sinkt[\"keep-open\"]\n"
        "\n"
        "_M.sink = _M.choose(sinkt)\n"
        "\n"
        "sourcet[\"by-length\"] = function(sock, length)\n"
        "    return base.setmetatable({\n"
        "        getfd = function() return sock:getfd() end,\n"
        "        dirty = function() return sock:dirty() end\n"
        "    }, {\n"
        "        __call = function()\n"
        "            if length <= 0 then return nil end\n"
        "            local size = math.min(socket.BLOCKSIZE, length)\n"
        "            local chunk, err = sock:receive(size)\n"
        "            if err then return nil, err end\n"
        "            length = length - string.len(chunk)\n"
        "            return chunk\n"
        "        end\n"
        "    })\n"
        "end\n"
        "\n"
        "sourcet[\"until-closed\"] = function(sock)\n"
        "    local done\n"
        "    return base.setmetatable({\n"
        "        getfd = function() return sock:getfd() end,\n"
        "        dirty = function() return sock:dirty() end\n"
        "    }, {\n"
        "        __call = function()\n"
        "            if done then return nil end\n"
        "            local chunk, err, partial = sock:receive(socket.BLOCKSIZE)\n"
        "            if not err then return chunk\n"
        "            elseif err == \"closed\" then\n"
        "                sock:close()\n"
        "                done = 1\n"
        "                return partial\n"
        "            else return nil, err end\n"
        "        end\n"
        "    })\n"
        "end\n"
        "\n"
        "\n"
        "sourcet[\"default\"] = sourcet[\"until-closed\"]\n"
        "\n"
        "_M.source = _M.choose(sourcet)\n"
        "\n"
        "return _M\n"
    ;
    luaL_dostring( lua_state, SOCKET_LUA );
    return 1;
#else
    (void) lua_state;
    return 0;
#endif
}
