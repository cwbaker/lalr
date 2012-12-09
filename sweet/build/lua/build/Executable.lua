
ExecutablePrototype = TargetPrototype { "Executable", BIND_PHONY };

function ExecutablePrototype.load_windows( executable )
    if build.load_target(executable) then
        if build.built_for_platform_and_variant(executable) then
            local link = Link( exe_name("%s/%s" % {executable.settings.bin, executable:id()}), executable );
            link.architecture = "";
            link.module = executable;
            executable:add_dependency( link );
        end
    end
end

function ExecutablePrototype.load_macosx( executable )
    if build.load_target(executable) then
        if build.built_for_platform_and_variant(executable) then
            local lipo = Lipo( exe_name("%s/%s" % {settings.bin, executable:id()}), executable );
            lipo.module = executable;
            executable:add_dependency( lipo );
        end
    end
end

function Executable( executable )
    local id = executable.id;
    executable.id = nil;
    return target( id, ExecutablePrototype, executable );
end

if operating_system() == "macosx" then
    ExecutablePrototype.load = ExecutablePrototype.load_macosx;
else
    ExecutablePrototype.load = ExecutablePrototype.load_windows;
end
