
#include <sweet/lua/Lua.hpp>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace sweet::lua;

static void print( const string& message )
{
    printf( "%s", message.c_str() );
}

void lua_hello_world_example()
{
    Lua lua;
    lua.globals()
        ( "print", &print )
    ;

    const char* script = "print('Hello World!\\n')";
    lua.call( script, script + strlen(script), "hello_world" ).end();
}
