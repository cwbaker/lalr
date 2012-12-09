
#include <sweet/lua/Lua.hpp>
#include <sweet/lua/vector.hpp>
#include <sweet/rtti/macros.hpp>
#include <string>
#include <string.h>

using std::string;
using std::vector;
using namespace sweet::lua;

struct Target
{
    vector<string> values_;
    
    Target()
    : values_()
    {
    }
    
    void add_value( const string& value )
    {
        values_.push_back( value );
    }
    
    const vector<string>& get_values() const
    {
        return values_;
    }
};

SWEET_LUA_TYPE_CONVERSION( Target, LuaByReference );

void lua_vector_example()
{
    Target target;
    target.add_value( "one" );
    target.add_value( "two" );
    target.add_value( "three" );

    Lua lua;
    lua.create( target );
    lua.members( target )
        .type( SWEET_STATIC_TYPEID(Target) )
        .this_pointer( &target )
        ( "get_values", &Target::get_values )
    ;    
    lua.globals()
        ( "target", target )
    ;
    
    const char* script = "for v in target:get_values() do print(v) end;";
    lua.call( script, script + strlen(script), "log" ).end();
}
