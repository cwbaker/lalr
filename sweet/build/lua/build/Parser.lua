
ParserPrototype = TargetPrototype { "Parser", BIND_PHONY };

function ParserPrototype.clone( self )
    return self;
end

function ParserPrototype.static_depend( self )
    if build.built_for_platform_and_variant(self) then        
        for _, value in ipairs(self) do
            local grammar = SourceFile( value );    
            grammar:set_required_to_exist( true );
            grammar.unit = self;
            local source = HeaderFile( basename(value)..".hpp" );
            source:set_bind_type( BIND_INTERMEDIATE_FILE );
            source.grammar = value;
            grammar.object = source;
            source:add_dependency( grammar );
            self:add_dependency( source );
        end
    end    
end

function ParserPrototype.build( self )
    if self:is_outdated() and build.built_for_platform_and_variant(self) then
        local parser = self.settings.parser.executable;
        for dependency in self:get_dependencies() do
            if dependency:is_outdated() then
                print( dependency.grammar );
                local arguments = "parser -r parser/cxx "..dependency.grammar;
                if exec(parser, arguments) ~= 0 then
                    error( arguments.." failed" );
                end
            end
        end
    end
end

function ParserPrototype.generate( self )
    self:build();
end

function ParserPrototype.clean( self )
    if build.built_for_platform_and_variant(self) and exists(settings.parser.executable) then    
        for dependency in self:get_dependencies() do
            if dependency:prototype() == HeaderFilePrototype then
                rm( dependency:path() );
            end
        end
    end
end

function Parser( parser )
    assert( type(parser) == "table" );
    return target( "", ParserPrototype, parser );
end

parser = {};

function parser.configure( settings )
    local local_settings = build.local_settings;
    if not local_settings.parser then
        local_settings.updated = true;
        if operating_system() == "windows" then
            local_settings.parser = {
                executable = "d:/usr/local/bin/parser.exe";
                lua_path = "d:/usr/local/lua/?.lua";
            };
        else
            local_settings.parser = {
                executable = "/usr/local/bin/parser.exe";
                lua_path = "/usr/local/lua/?.lua";
            };
        end
    end
end

function parser.initialize( settings )
    parser.configure( settings );
    putenv( "LUA_PATH", settings.parser.lua_path );
end
