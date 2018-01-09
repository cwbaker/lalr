
local Parser = build:TargetPrototype( "Parser" );

function Parser:build()
    -- Quietly do nothing if the parser executable is not available.
    local settings = self.settings;
    if build:exists(settings.parser.executable) then
        local command_line = {
            'parser',
            '-r parser/cxx',
            ('"%s"'):format( build:relative(self:dependency()) )
        };
        local environment = {
            LUA_PATH = settings.parser.lua_path;
        };
        local parser = settings.parser.executable;
        build:system( parser, command_line, environment );
    end
end

function Parser:clean()
end

function Parser:clobber()
    local settings = self.settings;
    if build:exists(settings.parser.executable) then    
        build:rm( self );
    end
end
