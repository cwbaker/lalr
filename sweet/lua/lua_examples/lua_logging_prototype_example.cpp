
#include <stdio.h>
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/LuaObject.hpp>

using namespace std;
using namespace sweet::lua;

enum LogMask
{
    LOG_NULL = 0x00,
    LOG_ERROR = 0x01,
    LOG_WARN = 0x02
};

class Log
{
    int level_;
    
    public:
        Log( int level )
        : level_( level )
        {
        }
        
        void set_level( int level )
        {
            level_ = level;
        }

        int get_level() const
        {
            return level_;
        }

        void log( int level, const std::string& format )
        {
            if ( level_ & level )
            {
                printf( "%s\n", format.c_str() );
            }
        }
};

SWEET_LUA_TYPE_CONVERSION( Log, LuaByReference );

void lua_logging_prototype_example()
{
    Lua lua;

    LuaObject log_prototype( lua );
    log_prototype.members()
        .type( SWEET_STATIC_TYPEID(Log) )
        ( "LOG_NULL", static_cast<int>(LOG_NULL) )
        ( "LOG_ERROR", static_cast<int>(LOG_ERROR) )
        ( "LOG_WARN", static_cast<int>(LOG_WARN) )
        ( "set_level", &Log::set_level )
        ( "get_level", &Log::get_level )
        ( "log", &Log::log )
    ;

    LuaObject log_metatable( lua );
    log_metatable.members()
        ( "__index", log_prototype )
    ;

    Log log( LOG_NULL );
    lua.create( log );
    lua.members( log )
        .metatable( log_metatable )
        .this_pointer( &log )
    ;
    
    lua.globals()
        ( "log", log )
    ;

    const char* script = 
        "log:set_level( log.LOG_ERROR + log.LOG_WARN );\n"
        "log:log( log.LOG_ERROR, 'This is an error' );\n"
        "log:log( log.LOG_WARN, 'This is a warning' );\n"
    ;

    lua.call( script, script + strlen(script), "logging" ).end();
}
