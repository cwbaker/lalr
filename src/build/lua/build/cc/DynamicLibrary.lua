
local DynamicLibrary = build:TargetPrototype( "DynamicLibrary" );

local function default_identifier_filename( identifier, architecture, settings )
    local settings = settings or build:current_settings();
    local identifier = build:interpolate( identifier, settings );
    local basename = build:basename( identifier );
    local branch = settings.bin;
    if build:is_absolute(identifier) then 
        branch = build:branch( identifier );
    end
    local filename = ("%s/%s"):format( branch, settings.dll_name(basename, architecture) );
    local identifier = ("%s_%s"):format( basename, architecture );
    return identifier, filename;
end

function DynamicLibrary.create( settings, identifier, architecture )
    local architecture = architecture or settings.architecture or settings.default_architecture;
    local identifier, filename = default_identifier_filename( identifier, architecture, settings );
    local dynamic_library = build:Target( identifier, DynamicLibrary );
    dynamic_library:set_filename( filename );
    dynamic_library:set_cleanable( true );
    dynamic_library:add_ordering_dependency( build:Directory(build:branch(dynamic_library)) );
    dynamic_library.settings = settings;
    dynamic_library.architecture = architecture or settings.default_architecture;
    return dynamic_library;
end

function DynamicLibrary:depend( dependencies )
    local libraries = dependencies.libraries;
    if libraries and platform ~= "" then
        local platform = self.settings.platform;
        local architecture = self.architecture;
        for _, value in ipairs(libraries) do
            local library = ("%s_%s_%s"):format( value, platform, architecture );
            self:add_dependency( build:target(build:root(library)) );
        end
    end
    return build.Target.depend( self, dependencies );
end

function DynamicLibrary:build()
    local settings = self.settings;
    settings.build_executable( self );
end
