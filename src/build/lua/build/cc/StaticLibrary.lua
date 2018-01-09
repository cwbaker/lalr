
local StaticLibrary = build.TargetPrototype( "StaticLibrary" );

-- Convert /filename/ into a library directory path be prepending the library
-- directory to the leaf of /filename/.
local function default_identifier_filename( identifier, architecture, settings )
    local settings = settings or build.current_settings();
    local identifier = ("%s_%s"):format( build.interpolate(identifier, settings), architecture );
    local basename = build.basename( identifier );
    local branch = settings.lib;
    if build.is_absolute(identifier) then 
        branch = build.branch( identifier );
    end
    -- The use of the global `lib_name()` here is temporary while the platform
    -- modules (e.g. macosx/init.lua etc) install their `lib_name()` functions
    -- into the globals table instead of the build table or somehow more 
    -- explicit.
    local filename = ("%s/%s"):format( branch, lib_name(basename, architecture) );
    return identifier, filename;
end

function StaticLibrary.create( settings, identifier, architecture )
    local identifier, filename = default_identifier_filename( identifier, architecture, settings );
    local static_library = build.Target( identifier, StaticLibrary );
    static_library:set_filename( filename );
    static_library:add_ordering_dependency( build.Directory(build.branch(static_library)) );
    static_library.settings = settings;
    static_library.architecture = architecture or settings.default_architecture
    return static_library;
end

function StaticLibrary:build()
    build_library( self );
end
