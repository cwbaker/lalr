
StaticLibraryPrototype = TargetPrototype { "StaticLibrary", BIND_PHONY };

function StaticLibraryPrototype.load_windows( static_library )
    if build.load_target(static_library) then
        if build.built_for_platform_and_variant(static_library) then
            local archive = Archive( "%s/%s" % {static_library.settings.lib, lib_name(static_library:id())}, static_library );
            archive.architecture = "";
            archive.module = static_library;
            static_library:add_dependency( archive );
        end
    end
end

function StaticLibraryPrototype.load_macosx( static_library )
    if build.load_target(static_library) then
        if build.built_for_platform_and_variant(static_library) then
            local architectures = static_library.architectures or static_library.settings.architectures;
            for _, architecture in ipairs(architectures) do 
                local archive = Archive( "%s/%s" % {static_library.settings.lib, lib_name("%s_%s" % {static_library:id(), architecture})}, static_library );
                archive.architecture = architecture;
                archive.module = static_library;
                static_library:add_dependency( archive );
            end
        end
    end
end

function StaticLibrary( static_library )
    local id = static_library.id;
    static_library.id = nil;
    return target( id, StaticLibraryPrototype, static_library );
end

if operating_system() == "macosx" then
    StaticLibraryPrototype.load = StaticLibraryPrototype.load_macosx;
else
    StaticLibraryPrototype.load = StaticLibraryPrototype.load_windows;
end
