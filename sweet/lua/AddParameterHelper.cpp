//
// AddParameterHelper.cpp
// Copyright (c) 2008 - 2011 Charles Baker.  All rights reserved.
//

#include "AddParameterHelper.hpp"
#include "Error.hpp"
#include "LuaStackGuard.hpp"
#include "LuaValue.hpp"
#include "Lua.hpp"
#include <sweet/error/macros.hpp>

using namespace sweet::lua;

/**
// Constructor.
//
// @param lua_state
//  The lua_State that this AddParameterHelper makes function calls in.
//
// @param lua
//  The Lua virtual machine that this AddParameterHelper is part of.
*/
AddParameterHelper::AddParameterHelper( lua_State* lua_state, Lua* lua )
: lua_state_( lua_state ),
  lua_( lua ),
  error_handler_( 0 ),
  parameters_( 0 ),
  call_type_( CALL_TYPE_NULL )
{
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( lua_ );
}

/**
// Load a Lua script from \e reader to a chunk on the top of the stack and 
// then call it.
//
// @param reader
//  The function to use to read data from.
//
// @param context
//  The context to pass to the reader function.
//
// @param name
//  The name to use when reporting errors.
//
// @param line
//  The line number to start numbering at (to include in debugging 
//  information).
*/
AddParameter AddParameterHelper::call( lua_Reader reader, void* context, const char* name )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_CALL;
    internal_begin( reader, context, name );
    return AddParameter( this );
}

AddParameter AddParameterHelper::call( const char* function )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_CALL;
    internal_begin( function );
    return AddParameter( this );
}

AddParameter AddParameterHelper::call( const LuaValue& function )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_CALL;
    internal_begin( function );
    return AddParameter( this );
}

AddParameter AddParameterHelper::resume( lua_Reader reader, void* context, const char* name )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_RESUME;
    internal_begin( reader, context, name );
    return AddParameter( this );
}

AddParameter AddParameterHelper::resume( const char* function )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_RESUME;
    internal_begin( function );
    return AddParameter( this );
}

AddParameter AddParameterHelper::resume( const LuaValue& function )
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_RESUME;
    internal_begin( function );
    return AddParameter( this );
}

/**
// Resume a call that previously yielded.
//
// @param function
//  The function to call.
//
// @return
//  An AddParameter helper object that provides a convenient syntax for 
//  passing parameters and receiving a return value.
*/
AddParameter AddParameterHelper::resume()
{
    parameters_ = 0;
    call_type_ = CALL_TYPE_RESUME;
    return AddParameter( this );
}

/**
// Call the function.
*/
void AddParameterHelper::end()
{
    LuaStackGuard stack_guard( lua_state_, error_handler_ == 0 ? parameters_ + 1 : parameters_ + 2 );
    internal_end( 0 );
}

/**
// Call the function and retrieve a boolean return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the boolean to place the return value into.
*/
void AddParameterHelper::end( bool* return_value )
{
    LuaStackGuard stack_guard( lua_state_, error_handler_ == 0 ? parameters_ + 1 : parameters_ + 2 );

    internal_end( 1 );
    if ( !lua_isboolean(lua_state_, -1) )
    {
        SWEET_ERROR( RuntimeError("Return value is not a boolean as expected") );
    }

    SWEET_ASSERT( return_value );
    *return_value = lua_toboolean( lua_state_, -1 ) == 1 ? true : false;
    lua_pop( lua_state_, 1 );
}

/**
// Call the function and retrieve an integer return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the int to place the return value into.
*/
void AddParameterHelper::end( int* return_value )
{
    LuaStackGuard stack_guard( lua_state_, error_handler_ == 0 ? parameters_ + 1 : parameters_ + 2 );

    internal_end( 1 );
    if ( !lua_isnumber(lua_state_, -1) )
    {
        SWEET_ERROR( RuntimeError("Return value is not a number as expected") );
    }

    SWEET_ASSERT( return_value );
    *return_value = lua_tointeger( lua_state_, -1 );
    lua_pop( lua_state_, 1 );
}

/**
// Call the function and retrieve a real return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the float to place the return value into.
*/
void AddParameterHelper::end( float* return_value )
{
    LuaStackGuard stack_guard( lua_state_, error_handler_ == 0 ? parameters_ + 1 : parameters_ + 2 );

    internal_end( 1 );
    if ( !lua_isnumber(lua_state_, -1) )
    {
        SWEET_ERROR( RuntimeError("Return value is not a number as expected") );
    }

    SWEET_ASSERT( return_value );
    *return_value = static_cast<float>( lua_tonumber(lua_state_, -1) );
    lua_pop( lua_state_, 1 );
}


/**
// Call the function and retrieve a string return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the string to place the return value into.
*/
void AddParameterHelper::end( std::string* return_value )
{
    LuaStackGuard stack_guard( lua_state_, error_handler_ == 0 ? parameters_ + 1 : parameters_ + 2 );

    internal_end( 1 );
    if ( !lua_isstring(lua_state_, -1) )
    {
        SWEET_ERROR( RuntimeError("Return value is not a string as expected") );
    }

    SWEET_ASSERT( return_value );
    *return_value = lua_tostring( lua_state_, -1 );
    lua_pop( lua_state_, 1 );
}

/**
// Call the function and retrieve an object return value.
//
// This is done in a separate function (instead of the destructor) because 
// calling the function can throw an exception.
//
// @param return_value
//  A pointer to the void pointer to place the return value into.
*/
void AddParameterHelper::end( void** return_value )
{
    LuaStackGuard stack_guard( lua_state_, error_handler_ == 0 ? parameters_ + 1 : parameters_ + 2 );

    internal_end( 1 );
    if ( !lua_isuserdata(lua_state_, -1) )
    {
        SWEET_ERROR( RuntimeError("Return value is not user data as expected") );
    }

    SWEET_ASSERT( return_value );
    *return_value = lua_touserdata( lua_state_, -1 );
    lua_pop( lua_state_, 1 );
}

/**
// Push a nil value onto the stack.
//
// @param nil
//  The LuaNil type that is used to represent a Lua nil value (ignored).
//
// @return
//  This AddParameter.
*/
void AddParameterHelper::push( const LuaNil& nil )
{
    lua_pushnil( lua_state_ );
    ++parameters_;
}

/**
// Push the global environment onto the stack.
//
// @param global_environment
//  The LuaGlobalEnvironment type that is used to represent the Lua global
//  environment (ignored).
//
// @return
//  This AddParameter.
*/
void AddParameterHelper::push( const LuaGlobalEnvironment& global_environment )
{
    lua_pushvalue( lua_state_, LUA_GLOBALSINDEX );
    ++parameters_;
}

/**
// Push a boolean value onto the stack.
//
// @param value
//  The boolean value to push.
//
// @return
//  This AddParameter.
*/
void AddParameterHelper::push( bool value )
{
    lua_pushboolean( lua_state_, value ? 1 : 0 );
    ++parameters_;
}

/**
// Push an integer value onto the stack.
//
// @param value
//  The integer value to push.
//
// @return
//  This AddParameter.
*/
void AddParameterHelper::push( int value )
{
    lua_pushinteger( lua_state_, static_cast<lua_Integer>(value) );
    ++parameters_;
}

/**
// Push a real value onto the stack.
//
// @param value
//  The real value to push.
//
// @return
//  This AddParameter.
*/
void AddParameterHelper::push( float value )
{
    lua_pushnumber( lua_state_, static_cast<lua_Number>(value) );
    ++parameters_;
}

/**
// Push a string value onto the stack.
//
// @param value
//  The string value to push.
//
// @return
//  This AddParameter.
*/
void AddParameterHelper::push( const char* value )
{
    lua_pushstring( lua_state_, value );
    ++parameters_;   
}

/**
// Push a string value onto the stack.
//
// @param value
//  The string value to push.
//
// @return
//  This AddParameter.
*/
void AddParameterHelper::push( const std::string& value )
{
    lua_pushlstring( lua_state_, value.c_str(), value.length() );
    ++parameters_;
}

/**
// Push the value referred to by \e value onto the stack.
//
// 
*/
void AddParameterHelper::push( const LuaValue& value )
{
    lua_push( lua_state_, value );
    ++parameters_;
}

/**
// Copy the values on the stack in the range [\e begin, \e end) onto the top 
// of the stack to act as parameters to this call.
//
// Relative offsets represented by negative stack indices cannot be passed as
// the value of \e begin or \e end.  They must be positive values.
//
// @param begin
//  The position on the stack to start copying values from (assumed >= 1).
//
// @param end
//  One past the last position on the stack to copy values from 
//  (assumed >= \e begin).
*/
void AddParameterHelper::copy_values_from_stack( int begin, int end )
{
    SWEET_ASSERT( begin >= 1 );
    SWEET_ASSERT( end >= begin );
    SWEET_ASSERT( lua_state_ );
    
    for ( int i = begin; i < end; ++i )
    {
        lua_pushvalue( lua_state_, i );
        ++parameters_;
    }
}

/**
// Call a chunk at the top of the Lua stack.
//
// @param reader
//  The lua_Reader function to call to read the chunk in.
//
// @param context
//  The context to pass to the reader function.
//
// @param name
//  The name of the chunk for debug information.
*/
void AddParameterHelper::internal_begin( lua_Reader reader, void* context, const char* name )
{
    SWEET_ASSERT( reader );
    SWEET_ASSERT( context );
    SWEET_ASSERT( name );
    SWEET_ASSERT( lua_ );
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( error_handler_ == 0 );

    LuaStackGuard stack_guard( lua_state_ );
    
    if ( call_type_ == CALL_TYPE_CALL && lua_->is_stack_trace_enabled() )
    {
        lua_pushcfunction( lua_state_, &AddParameterHelper::stack_trace_for_call );
        error_handler_ = lua_gettop( lua_state_ );
    }
    else
    {
        error_handler_ = 0;
    }

    int result = lua_load( lua_state_, reader, context, name );
    switch ( result )
    {
        case 0:
            break;

        case LUA_ERRSYNTAX:
            error_handler_ = 0;
            SWEET_ERROR( SyntaxError("Syntax error loading '%s' - %s", name, lua_tostring(lua_state_, -1)) );
            break;

        case LUA_ERRMEM:
            error_handler_ = 0;
            SWEET_ERROR( MemoryAllocationError("Memory allocation error loading '%s' - %s", name, lua_tostring(lua_state_, -1)) );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }

    if ( !lua_isfunction(lua_state_, -1) )
    {
        error_handler_ = 0;
        SWEET_ERROR( RuntimeError("The value at the top of the stack is not a function") );
    }
    
    stack_guard.reset_to_top_of_stack();
}

/**
// Call a global function.
//
// @param function
//  The name of the function to call.
*/
void AddParameterHelper::internal_begin( const char* function )
{
    SWEET_ASSERT( function );
    SWEET_ASSERT( lua_ );
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( error_handler_ == 0 );

    LuaStackGuard stack_guard( lua_state_ );

    if ( call_type_ == CALL_TYPE_CALL && lua_->is_stack_trace_enabled() )
    {
        lua_pushcfunction( lua_state_, &AddParameterHelper::stack_trace_for_call );
        error_handler_ = lua_gettop( lua_state_ );
    }
    else
    {
        error_handler_ = 0;
    }

    lua_getglobal( lua_state_, function );
    if ( !lua_isfunction(lua_state_, -1) && !lua_istable(lua_state_, -1) )
    {
        error_handler_ = 0;
        SWEET_ERROR( RuntimeError("The global variable '%s' is not a function or a table", function) );
    }
    
    stack_guard.reset_to_top_of_stack();
}

void AddParameterHelper::internal_begin( const LuaValue& function )
{
    SWEET_ASSERT( lua_ );
    SWEET_ASSERT( lua_state_ );
    SWEET_ASSERT( error_handler_ == 0 );

    LuaStackGuard stack_guard( lua_state_ );

    if ( call_type_ == CALL_TYPE_CALL && lua_->is_stack_trace_enabled() )
    {
        lua_pushcfunction( lua_state_, &AddParameterHelper::stack_trace_for_call );
        error_handler_ = lua_gettop( lua_state_ );
    }
    else
    {
        error_handler_ = 0;
    }

    lua_push( lua_state_, function );
    if ( !lua_isfunction(lua_state_, -1) && !lua_istable(lua_state_, -1) )
    {
        error_handler_ = 0;
        SWEET_ERROR( RuntimeError("The value referenced is not a function or a table") );
    }
    
    stack_guard.reset_to_top_of_stack();
}

/**
// Finish a call or a resume.
//
// @param results
//  The number of expected results.
*/
void AddParameterHelper::internal_end( int results )
{
    int parameters = parameters_;
    int error_handler = error_handler_;
    CallType call_type = call_type_;

    parameters_ = 0;
    error_handler_ = 0;
    call_type_ = CALL_TYPE_NULL;

    switch ( call_type )
    {
        case CALL_TYPE_CALL:
            internal_end_call( parameters, results, error_handler );
            break;

        case CALL_TYPE_RESUME:
            internal_end_resume( parameters );
            break;

        default:
            SWEET_ASSERT( false );
            break;
    }
}

/**
// Finish a call.
//
// @param results
//  The number of expected results.
*/
void AddParameterHelper::internal_end_call( int parameters, int results, int error_handler )
{
    SWEET_ASSERT( parameters >= 0 );    
    SWEET_ASSERT( results == 0 || results == 1 );
    SWEET_ASSERT( error_handler >= 0 );
    SWEET_ASSERT( lua_state_ );

    int result = lua_pcall( lua_state_, parameters, results, error_handler );
    switch ( result )
    {
        case 0:
            break;

        case LUA_ERRRUN:
            SWEET_ERROR( RuntimeError("%s", lua_tostring(lua_state_, -1)) );
            break;

        case LUA_ERRMEM:
            SWEET_ERROR( MemoryAllocationError("Out of memory - %s", lua_tostring(lua_state_, -1)) );
            break;

        case LUA_ERRERR:
            SWEET_ERROR( RuntimeError("Error handler failed - %s", lua_tostring(lua_state_, -1)) );
            break;

        case -1:
            SWEET_ERROR( RuntimeError("Execution failed due to an unhandled C++ exception") );
            break;

        default:
            SWEET_ASSERT( false );
            SWEET_ERROR( RuntimeError("Execution failed in an unexpected way - %s", lua_tostring(lua_state_, -1)) );
            break;
    }
}

/**
// Finish a resume.
//
// @param parameters
//  The number of parameters that have been pushed onto the stack to be passed
//  to the function.
*/
void AddParameterHelper::internal_end_resume( int parameters )
{
    int result = lua_resume( lua_state_, parameters );
    switch ( result )
    {
        case 0:
            break;

        case LUA_YIELD:
            break;            

        case LUA_ERRRUN:
        {
            char message [1024];                
            SWEET_ERROR( RuntimeError("%s", stack_trace_for_resume(lua_state_, lua_->is_stack_trace_enabled(), message, sizeof(message))) );
            break;
        }

        case LUA_ERRMEM:
        {
            char message [1024];
            SWEET_ERROR( MemoryAllocationError("Out of memory - %s", stack_trace_for_resume(lua_state_, lua_->is_stack_trace_enabled(), message, sizeof(message))) );
            break;
        }

        case LUA_ERRERR:
        {
            char message [1024];
            SWEET_ERROR( RuntimeError("Error handler failed - %s", stack_trace_for_resume(lua_state_, lua_->is_stack_trace_enabled(), message, sizeof(message))) );
            break;
        }
        
        case -1:
        {
            char message [1024];
            SWEET_ERROR( RuntimeError("Execution failed due to an unhandled C++ exception - %s", stack_trace_for_resume(lua_state_, lua_->is_stack_trace_enabled(), message, sizeof(message))) );
            break;
        }

        default:
        {
            SWEET_ASSERT( false );
            char message [1024];
            SWEET_ERROR( RuntimeError("Execution failed in an unexpected way - %s", stack_trace_for_resume(lua_state_, lua_->is_stack_trace_enabled(), message, sizeof(message))) );
            break;
        }
    }
}

/**
// Do a Lua stack trace.
//
// This function is pushed onto the stack for each Lua and LuaThread object 
// that is created.  Its position is passed to lua_pcall() when this 
// AddParameter object makes its call to Lua.  If an error occurs during the 
// execution of the call this function is called with the string describing 
// the error at the top of the stack.
//
// This function then walks back up the Lua stack recording each function that
// has been called from the error occuring back up to the first call into
// the Lua virtual machine from C++.
//
// It then returns all of this information concatenated into a single string 
// that is passed back to the application as the value at the top of the Lua
// stack.
//
// @param lua_state
//  The lua_State to do a stack trace for.
//
// @return
//  Always returns 1.
*/
int AddParameterHelper::stack_trace_for_call( lua_State* lua_state )
{
    const int STACK_TRACE_BEGIN = 0;
    const int STACK_TRACE_END = 6;

    lua_Debug debug;
    memset( &debug, 0, sizeof(debug) );
    
    lua_pushliteral( lua_state, ".\nstack trace:" );

    int level = STACK_TRACE_BEGIN;
    while ( level < STACK_TRACE_END && lua_getstack(lua_state, level, &debug) )
    {
        lua_getinfo( lua_state, "Snl", &debug );
    
    //
    // Source and line number.
    //
        lua_pushliteral( lua_state, "\n  " );        
        if ( debug.currentline > 0 )
        {
            lua_pushfstring( lua_state, "%s(%d) : ", debug.source, debug.currentline );
        }
        else
        {
            lua_pushfstring( lua_state, "%s(1) : ", debug.source );
        }

    //
    // Application provided name or other implicit location information.
    //
        if ( *debug.namewhat != '\0' )
        {
            lua_pushfstring( lua_state, "in function " LUA_QS, debug.name );
        }
        else 
        {
            switch ( *debug.what )
            {
                case 'm':
                    lua_pushfstring( lua_state, "main");
                    break;

                default:
                    lua_pushfstring( lua_state, "in function <%s(%d)>", debug.source, debug.linedefined );
            }
        }

        lua_concat( lua_state, lua_gettop(lua_state) );
        ++level;
    }

    lua_concat( lua_state, lua_gettop(lua_state) );
    return 1;
}

/**
// Do a Lua stack trace after lua_resume() has been called to call a function
// and that function has failed.
//
// This function walks back up the Lua stack recording each function that had 
// been called from the error occuring back up to the first call into
// Lua from C++.
//
// @param lua_state
//  The lua_State to do a stack trace for.
//
// @param stack_trace_enabled
//  True to do a stack trace or false to just retrieve the error message from
//  the top of the stack.
//
// @param message
//  The buffer to put the stack trace message into.
//
// @param length
//  The maximum length of the stack trace message (in bytes).
//
// @return
//  Returns the string passed in \e message for convenience.
*/
const char* AddParameterHelper::stack_trace_for_resume( lua_State* lua_state, bool stack_trace_enabled, char* message, size_t length )
{
    SWEET_ASSERT( lua_state != NULL );
    SWEET_ASSERT( message != NULL );
    SWEET_ASSERT( length > 0 );

    size_t written = 0;
    memset( message, 0, length );

    written += _snprintf( message + written, length - written, "%s", lua_isstring(lua_state, -1) ? lua_tostring(lua_state, -1) : "Unknown error" );

    if ( stack_trace_enabled )
    {
        static const int STACK_TRACE_BEGIN = 0;
        static const int STACK_TRACE_END   = 6;

        written += _snprintf( message + written, length - written, ".\nstack trace:" );

        lua_Debug debug;
        memset( &debug, 0, sizeof(debug) );

        int level = STACK_TRACE_BEGIN;
        while ( level < STACK_TRACE_END && lua_getstack(lua_state, level, &debug) )
        {
            lua_getinfo( lua_state, "Snl", &debug );
        
        //
        // Source and line number.
        //
            written += _snprintf( message + written, length - written, "\n  " );        
            if ( debug.currentline > 0 )
            {
                written += _snprintf( message + written, length - written, "%s(%d) : ", debug.source, debug.currentline );
            }
            else
            {
                written += _snprintf( message + written, length - written, "%s(1) : ", debug.source, debug.currentline );
            }

        //
        // Application provided name or other implicit location information.
        //
            if ( *debug.namewhat != '\0' )
            {
                written += _snprintf( message + written, length - written, "in function " LUA_QS, debug.name );
            }
            else 
            {
                switch ( *debug.what )
                {
                    case 'm':
                        written += _snprintf( message + written, length - written, "main");
                        break;

                    default:
                        written += _snprintf( message + written, length - written, "in function <%s(%d)>", debug.source, debug.linedefined );
                        break;
                }
            }

            ++level;
        }
    }

    message [length - 1] = 0;
    return message;
}
