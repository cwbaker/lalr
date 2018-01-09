
DynamicLibraryPrototype = TargetPrototype { "DynamicLibrary", BIND_PHONY };

function DynamicLibraryPrototype.load_windows( dynamic_library )
    if build.load_target(dynamic_library) then
        if build.built_for_platform_and_variant(dynamic_library) then
            local link = Link( dll_name("%s/%s" % {dynamic_library.settings.bin, dynamic_library:id()}), dynamic_library );
            link.architecture = "";
            link.module = dynamic_library;
            link:add_dependency( Directory(dynamic_library.settings.lib) );
            dynamic_library:add_dependency( link );
        end
    end
end

function DynamicLibraryPrototype.load_macosx( dynamic_library )
    if build.load_target(dynamic_library) then
        if build.built_for_platform_and_variant(dynamic_library) then
            local lipo = Lipo( dll_name("%s/%s" % {dynamic_library.settings.bin, dynamic_library:id()}), dynamic_library );
            lipo.module = dynamic_library;
            dynamic_library:add_dependency( lipo );
        end
    end
end

function DynamicLibrary( dynamic_library )
    local id = dynamic_library.id;
    dynamic_library.id = nil;
    return target( id, DynamicLibraryPrototype, dynamic_library );
end

if operating_system() == "macosx" then
    DynamicLibraryPrototype.load = DynamicLibraryPrototype.load_macosx;
else
    DynamicLibraryPrototype.load = DynamicLibraryPrototype.load_windows;
end
