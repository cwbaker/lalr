
parser = {};

function parser.configure( settings )
    local local_settings = build.local_settings;
    if not local_settings.parser then
        local_settings.updated = true;
        if build.operating_system() == "windows" then
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

require "build.parser.Parser";

build.register_module( parser );
