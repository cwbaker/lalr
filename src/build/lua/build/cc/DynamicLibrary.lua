
local DynamicLibrary = build.TargetPrototype( "DynamicLibrary" );

local function default_identifier_filename( identifier, architecture, settings )
    local settings = settings or build.current_settings();
    local identifier = build.interpolate( identifier, settings );
    local basename = build.basename( identifier );
    local branch = settings.bin;
    if build.is_absolute(identifier) then 
        branch = build.branch( identifier );
    end

    -- The use of the global `dll_name()` here is temporary while the platform
    -- modules (e.g. macosx/init.lua etc) install their `dll_name()` functions
    -- into the globals table instead of the build table or somehow more 
    -- explicit.
    local filename = ("%s/%s"):format( branch, dll_name(basename, architecture) );

    return identifier, filename;
end

function DynamicLibrary.create( settings, identifier, architecture )
    local identifier, filename = default_identifier_filename( identifier, architecture, settings );
    local dynamic_library = build.Target( identifier, DynamicLibrary );
    dynamic_library:set_filename( filename );
    dynamic_library:set_cleanable( true );
    dynamic_library:add_ordering_dependency( build.Directory(build.branch(dynamic_library)) );
    dynamic_library.settings = settings;
    dynamic_library.architecture = architecture or settings.default_architecture;
    return dynamic_library;
end

function DynamicLibrary:depend( dependencies )
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

function DynamicLibrary:build()
    build_executable( self );
end
