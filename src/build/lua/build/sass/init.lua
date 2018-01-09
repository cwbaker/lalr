
sass = {};

function sass.configure( settings )
    local local_settings = build.local_settings;
    if not local_settings.sass then
        local_settings.updated = true;
        local_settings.sass = {
            executable = "/usr/bin/sass";
        };
     end
end

function sass.initialize( settings )
end

require "build.sass.Css";

build:register_module( sass );
