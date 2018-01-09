
#include <sweet/lua/Lua.hpp>
#include <sweet/error/ErrorPolicy.hpp>
#include <string.h>

using namespace sweet;
using namespace sweet::lua;

void lua_factorial_example()
{
    error::ErrorPolicy error_policy;
    Lua lua( error_policy );

    const char* script = 
        "function factorial(n) \n"
        "   if n <= 1 then \n"
        "       return 1 \n"
        "   else \n"
        "       return n * factorial(n - 1) \n"
        "   end \n"
        "end"
    ;
    lua.call( script, script + strlen(script), "factorial" )
    .end();

    int factorial = 0;
    lua.call( "factorial" )
        ( 4 )
    .end( &factorial );
    SWEET_ASSERT( factorial == 24 );
}
