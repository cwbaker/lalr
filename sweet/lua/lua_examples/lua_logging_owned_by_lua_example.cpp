
#if defined SWEET_BOOST_ENABLED

#include <sweet/lua.hpp>
#include <sweet/lua/shared_ptr.hpp>

using namespace boost;
using namespace sweet::lua;

class Log;
SWEET_LUA_TYPE_CONVERSION( Log, LuaReference );

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


struct LogCreator
{
    Lua* lua_;
    LuaObject* log_metatable_;
    
    LogCreator( Lua* lua, LuaObject* log_metatable )
    : lua_( lua ),
      log_metatable_( log_metatable )
    {
    }       

    shared_ptr<Log> create()
    {
        struct LogDeleter
        {
            Lua* lua_;

            LogDeleter( Lua* lua )
            : lua_( lua )
            {
            }
            
            void operator()( Log* log ) const
            {
                lua_->destroy( *log );
                delete log;
                log = NULL;
            }
        };

        shared_ptr<Log> log( new Log(LOG_NULL), LogDeleter(lua_) );
        lua_->create( *log );
        lua_->members( *log )
            .metatable( *log_metatable_ )
            .this_pointer( log.get() )
            ( "ptr", value(log) )
        ;
        return log;
    }
};

void lua_logging_owned_by_lua_example()
{
    Lua lua;

    LuaObject log_prototype( lua );
    log_prototype.members()
        .type( SWEET_STATIC_TYPEID(Log) )
        ( "LOG_NULL", static_cast<int>(LOG_NULL)  )
        ( "LOG_ERROR", static_cast<int>(LOG_ERROR) )
        ( "LOG_WARN", static_cast<int>(LOG_WARN)  )
        ( "set_level", &Log::set_level )
        ( "get_level", &Log::get_level )
        ( "log", &Log::log       )
    ;

    LuaObject log_metatable( lua );
    log_metatable.members()
        .type( SWEET_STATIC_TYPEID(LuaObject) )
        ( "__index", log_prototype )
    ;

    LogCreator log_creator( &lua, &log_metatable );
    lua.globals()
        ( "Log", weaken(&LogCreator::create), &log_creator )
    ;
    
    const char* script = 
        "local log = Log();\n"
        "log:set_level( log.LOG_ERROR + log.LOG_WARN );\n"
        "log:log( log.LOG_ERROR, 'This is an error' );\n"
        "log:log( log.LOG_WARN, 'This is a warning' );\n"
    ;    
    lua.call( script, script + strlen(script), "logging", 0 ).end();
}

#else

void lua_logging_owned_by_lua_example()
{
}

#endif
