
local Parser = build.TargetPrototype( "Parser" );

function Parser.create( settings, value )
    local grammar = build.SourceFile( value );    
    local parser = build.GeneratedFile( ("%s.hpp"):format(build.basename(value)), Parser );
    parser.grammar = value;
    parser.settings = settings;
    parser:add_dependency( grammar );
    parser:build();
    return parser;
end

function Parser.build( parser )
    local parser_ = parser.settings.parser.executable;
    if not build.exists(parser:filename()) or parser:outdated() then
        print( parser.grammar );
        local environment = {
            LUA_PATH = settings.parser.lua_path;
        };
        local arguments = ("parser -r parser/cxx %s"):format( parser.grammar );
        local result = build.execute( parser_, arguments, environment );
        assertf( result == 0, ("[[%s]] failed"):format(arguments) );
    end
end

function Parser.clean( parser )
end

function Parser.clobber( parser )
    if build.exists(parser.settings.parser.executable) then    
        build.rm( parser:filename() );
    end
end
