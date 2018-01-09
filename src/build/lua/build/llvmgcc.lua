
llvmgcc = {};

function llvmgcc.configure( settings )
    local local_settings = build.local_settings;
    if not local_settings.llvmgcc then
        local_settings.updated = true;
        local_settings.llvmgcc = {
            xcrun = "/usr/bin/xcrun";
        };
    end
end;

function llvmgcc.initialize( settings )
    llvmgcc.configure( settings );

    if platform == "llvmgcc" then
        cc = llvmgcc.cc;
        objc = llvmgcc.objc;
        build_library = llvmgcc.build_library;
        clean_library = llvmgcc.clean_library;
        build_executable = llvmgcc.build_executable;
        clean_executable = llvmgcc.clean_executable;
        lipo_executable = llvmgcc.lipo_executable;
        obj_directory = llvmgcc.obj_directory;
        cc_name = llvmgcc.cc_name;
        cxx_name = llvmgcc.cxx_name;
        obj_name = llvmgcc.obj_name;
        lib_name = llvmgcc.lib_name;
        dll_name = llvmgcc.dll_name;
        exe_name = llvmgcc.exe_name;
    end
end;

function llvmgcc.cc( target, definition )
    local cppdefines = "";
    cppdefines = cppdefines.." -DBUILD_OS_MACOSX";
    cppdefines = cppdefines.." -DBUILD_PLATFORM_"..upper(platform);
    cppdefines = cppdefines.." -DBUILD_VARIANT_"..upper(variant);
    cppdefines = cppdefines.." -DBUILD_LIBRARY_SUFFIX=\"\\\"_"..platform.."_"..variant..".lib\\\"\"";
    cppdefines = cppdefines.." -DBUILD_MODULE_"..upper(string.gsub(target.module:id(), "-", "_"))
    cppdefines = cppdefines.." -DBUILD_LIBRARY_TYPE_"..upper(definition.settings.library_type);

    if definition.settings.defines then
        for _, define in ipairs(definition.settings.defines) do
            cppdefines = cppdefines.." -D"..define;
        end
    end
    
    if definition.defines then
        for _, define in ipairs(definition.defines) do
            cppdefines = cppdefines.." -D"..define;
        end
    end

    local cppdirs = "";
    if definition.include_directories then
        for _, directory in ipairs(definition.include_directories) do
            cppdirs = cppdirs.." -I\""..relative(directory).."\"";
        end
    end

    if definition.settings.include_directories then
        for _, directory in ipairs(definition.settings.include_directories) do
            cppdirs = cppdirs.." -I\""..directory.."\"";
        end
    end

    local ccflags = "";
    ccflags = [["%s -c]] % ccflags;
    ccflags = [["%s -arch %s]] % { ccflags, target.architecture };
    
    if definition.settings.compile_as_c then
        ccflags = ccflags.." -x c";
    else
        ccflags = ccflags.." -fpermissive -Wno-deprecated -x c++";
    end
    
    if definition.settings.runtime_library == "static" or definition.settings.runtime_library == "static_debug" then
        ccflags = ccflags.." -static-libstdc++";
    end
    
    if definition.settings.debug then
        ccflags = ccflags.." -g";
    end

    if definition.settings.optimization then
        ccflags = ccflags.." -O2";
    end
    
    if definition.settings.preprocess then
        ccflags = ccflags.." -E";
    end

    if definition.settings.exceptions and not definition.settings.compile_as_c then
        ccflags = ccflags.." -fexceptions";
    end

    if definition.settings.run_time_type_info and not definition.settings.compile_as_c then
        ccflags = ccflags.." -frtti";
    end

    if definition.settings.runtime_checks then
        ccflags = [[%s -fstack-protector]] % ccflags;
    else
        ccflags = [[%s -fno-stack-protector]] % ccflags;
    end

    if target.precompiled_header ~= nil then            
        if target.precompiled_header:is_outdated() then
            print( leaf(target.precompiled_header.source) );
            local xcrun = definition.settings.llvmgcc.xcrun;
            build.system( xcrun, "xcrun g++ %s %s %s -o %s %s" % {cppdirs, cppdefines, ccflags, target.precompiled_header:get_filename(), target.precompiled_header.source} );
        end        
    end
    
    cppdefines = cppdefines.." -DBUILD_VERSION=\"\\\""..version.."\\\"\"";
    for dependency in target:get_dependencies() do
        if dependency:is_outdated() and dependency ~= target.precompiled_header then
            if dependency:prototype() == FilePrototype then
                print( leaf(dependency.source) );
                local xcrun = definition.settings.llvmgcc.xcrun;
                build.system( xcrun, "xcrun g++ %s %s %s -o %s %s" % {cppdirs, cppdefines, ccflags, dependency:get_filename(), absolute(dependency.source)} );
            elseif dependency.results then
                for _, result in ipairs(dependency.results) do
                    if result:is_outdated() then
                        print( leaf(result.source) );
                        local xcrun = definition.settings.llvmgcc.xcrun;
                        build.system( xcrun, "xcrun g++ %s %s %s -o %s %s" % {cppdirs, cppdefines, ccflags, result:get_filename(), absolute(result.source)} );
                    end
                end
            end
        end    
    end
end;

function llvmgcc.build_library( target, definition )
    local arflags = "";
    arflags = [[%s -static]] % arflags;

    local objects = "";
    for compile in target:get_dependencies() do
        if compile:prototype() == CompilePrototype then
            if compile.precompiled_header then
                objects = [[%s %s]] % { objects, leaf(compile.precompiled_header:get_filename()) };
            end
            
            for object in compile:get_dependencies() do
                if object:prototype() == FilePrototype and object ~= compile.precompiled_header then
                    objects = [[%s %s]] % { objects, leaf(object:get_filename()) };
                end
            end
        end
    end
    
    if objects ~= "" then
        print( leaf(target:get_filename()) );
        pushd( obj_directory(target.module) );
        local xcrun = definition.settings.llvmgcc.xcrun;
        build.system( xcrun, [[xcrun libtool %s -o %s %s]] % {arflags, native(target:get_filename()), objects} );
        popd();
    end
end;

function llvmgcc.clean_library( target, definition )
    rm( target:get_filename() );
    rmdir( obj_directory(target.module) );
end;

function llvmgcc.build_executable( target, definition )
    local ldlibs = " ";
    
    local lddirs = " -L \""..definition.settings.lib.."\"";

    if definition.settings.library_directories then
        for _, directory in ipairs(definition.settings.library_directories) do
            lddirs = lddirs.." -L \""..directory.."\"";
        end
    end
    
    local architecture = target.architecture;

    local ldflags = "";
    ldflags = [[%s -arch %s]] % { ldflags, architecture };

    ldflags = [[%s -o %s]] % { ldflags, native(target:get_filename()) };
    if target:prototype() == ArchivePrototype then
        ldflags = ldflags.." -shared -Wl,--out-implib,"..native( definition.settings.lib.."/"..lib_name(target:id()) );
    end
    
    if definition.settings.verbose_linking then
        ldflags = ldflags.." -Wl,--verbose=31";
    end
    
    if definition.settings.runtime_library == "static" or definition.settings.runtime_library == "static_debug" then
        ldflags = ldflags.." -static-libstdc++";
    end
    
    if definition.settings.debug then
        ldflags = ldflags.." -debug";
    end

    if definition.settings.strip then
        ldflags = ldflags.." -Wl,-dead_strip";
    end

    local libraries = "";
    if target.libraries then
        for _, library in ipairs(target.libraries) do
            libraries = "%s -l%s" % { libraries, string.gsub(basename(library:id()), "lib", "", 1) };
        end
    end
    if definition.third_party_libraries then
        for _, library in ipairs(definition.third_party_libraries) do
            libraries = "%s -l%s" % { libraries, library };
        end
    end
    if definition.system_libraries then
        for _, library in ipairs(definition.system_libraries) do 
            libraries = "%s -l%s" % { libraries, library };
        end
    end
    if definition.frameworks then
        for _, framework in ipairs(definition.frameworks) do
            libraries = "%s -framework %s" % { libraries, framework };
        end
    end

    local objects = "";
    for dependency in target:get_dependencies() do
        if dependency:prototype() == CompilePrototype then
            if dependency.precompiled_header then
                objects = [[%s %s]] % { objects, leaf(dependency.precompiled_header:get_filename()) };
            end
            
            for object in dependency:get_dependencies() do
                if object:prototype() == FilePrototype and object ~= dependency.precompiled_header then
                    objects = [[%s %s]] % { objects, leaf(object:get_filename()) };
                end
            end
        end
    end

    if objects ~= "" then
        print( leaf(target:get_filename()) );
        pushd( obj_directory(target.module) );
        local xcrun = definition.settings.llvmgcc.xcrun;
        build.system( xcrun, "xcrun g++"..ldflags..lddirs..objects..libraries..ldlibs );
        popd();
    end
end;

function llvmgcc.clean_executable( target, definition )
    rm( target:get_filename() );
    rmdir( obj_directory(target.module) );
end;

function llvmgcc.lipo_executable( target, definition )
    local executables = "";
    for executable in target:get_dependencies() do 
        if executable:prototype() == LinkPrototype then
            executables = [[%s %s]] % { executables, executable:get_filename() };
        end
    end
    print( leaf(target:get_filename()) );
    local xcrun = definition.settings.llvmgcc.xcrun;
    build.system( xcrun, [[xcrun lipo -create %s -output %s]] % {executables, target:get_filename()} );
end

function llvmgcc.obj_directory( target )
    return "%s/%s_%s/%s" % { target.settings.obj, platform, variant, relative(target:directory(), root()) };
end;

function llvmgcc.cc_name( name )
    return "%s.c" % basename( name );
end;

function llvmgcc.cxx_name( name )
    return "%s.cpp" % basename( name );
end;

function llvmgcc.obj_name( name, architecture )
    return "%s_%s.o" % { basename(name), architecture };
end;

function llvmgcc.lib_name( name )
    return "lib%s_%s.a" % { name, variant };
end;

function llvmgcc.dll_name( name )
    return "%s_%s.dylib" % { name, variant };
end;

function llvmgcc.exe_name( name )
    return "%s_%s" % { name, variant };
end;
