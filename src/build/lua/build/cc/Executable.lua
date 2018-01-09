
local Executable = build.TargetPrototype( "Executable" );

local function default_identifier_filename( identifier, architecture, settings )
    local settings = settings or build.current_settings();
    local identifier = build.interpolate( identifier, settings );
    local basename = build.basename( identifier );
    local branch = settings.bin;
    if build.is_absolute(identifier) then 
        branch = build.branch( identifier );
    end

    -- The use of the global `exe_name()` here is temporary while the platform
    -- modules (e.g. macosx/init.lua etc) install their `exe_name()` functions
    -- into the globals table instead of the build table or somehow more 
    -- explicit.
    local filename = ("%s/%s"):format( branch, exe_name(basename, architecture) );

    return identifier, filename;
end

function Executable.create( settings, identifier, architecture )
    local identifier, filename = default_identifier_filename( identifier, architecture, settings );
    local executable = build.Target( identifier, Executable );
    executable:set_filename( filename );
    executable:set_cleanable( true );
    executable:add_ordering_dependency( build.Directory(build.branch(executable)) );
    executable.settings = settings;
    executable.architecture = architecture or settings.default_architecture;
    return executable;
end

function Executable:depend( dependencies )
    local libraries = dependencies.libraries;
    if libraries and platform ~= "" then
        local architecture = self.architecture;
        for _, value in ipairs(libraries) do
            local library = ("%s_%s"):format( value, architecture );
            self:add_dependency( build.target(build.root(library)) );
        end
    end
    return build.default_depend( self, dependencies );
end

function Executable:build()
    build_executable( self );
end
