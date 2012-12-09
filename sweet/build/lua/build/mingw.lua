
mingw = {};

function mingw.configure( settings )
    function autodetect_mingw_directory()
        local mingw_directory = "C:/MinGW";
        return mingw_directory;
    end

    local local_settings = build.local_settings;
    if not local_settings.mingw then
        local_settings.updated = true;
        local_settings.mingw = {
            mingw_directory = autodetect_mingw_directory() or "C:/MinGW";
        };
    end
end

function mingw.initialize( settings )
    mingw.configure( settings );
    if platform == "mingw" then
        -- Make sure that the environment variable VS_UNICODE_OUTPUT is not set.  
        -- Visual Studio sets this to signal its tools to communicate back to 
        -- Visual Studio using named pipes rather than stdout so that unicode output 
        -- works better but this then prevents the build tool from intercepting
        -- and collating this output.
        -- See http://blogs.msdn.com/freik/archive/2006/04/05/569025.aspx.
        putenv( "VS_UNICODE_OUTPUT", "" );

        local mingw_directory = settings.mingw.mingw_directory;
        
        local path = {
            "%s/bin" % mingw_directory,
            getenv("PATH")
        };
        putenv( "PATH", table.concat(path, ";") );
        
        local include = {
            "%s/include" % mingw_directory,
            getenv("INCLUDE")
        };
        putenv( "INCLUDE", table.concat(include, ";") );
        
        local lib = {
            "%s/lib" % mingw_directory,
            getenv("LIB")
        };
        putenv( "LIB", table.concat(lib, ";") );

        cc = mingw.cc;
        build_library = mingw.build_library;
        clean_library = mingw.clean_library;
        build_executable = mingw.build_executable;
        clean_executable = mingw.clean_executable;
        obj_directory = mingw.obj_directory;
        cc_name = mingw.cc_name;
        cxx_name = mingw.cxx_name;
        pch_name = mingw.pch_name;
        pdb_name = mingw.pdb_name;
        obj_name = mingw.obj_name;
        lib_name = mingw.lib_name;
        exp_name = mingw.exp_name;
        dll_name = mingw.dll_name;
        exe_name = mingw.exe_name;
        ilk_name = mingw.ilk_name;
    end
end

function mingw.cc( target, definition )
    local gcc = "%s/bin/gcc.exe" % definition.settings.mingw.mingw_directory;

    local cppdefines = "";
    cppdefines = cppdefines.." -DBUILD_OS_"..upper(operating_system());
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
    
    if target.defines then
        for _, define in ipairs(target.defines) do
            cppdefines = cppdefines.." -D"..define;
        end
    end

    local cppdirs = "";
    if target.include_directories then
        for _, directory in ipairs(target.include_directories) do
            cppdirs = cppdirs.." -I\""..relative(directory).."\"";
        end
    end

    if definition.settings.include_directories then
        for _, directory in ipairs(definition.settings.include_directories) do
            cppdirs = cppdirs.." -I\""..directory.."\"";
        end
    end

    local ccflags = " -c";

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

    if target.precompiled_header ~= nil then            
        if target.precompiled_header:is_outdated() then
            print( leaf(target.precompiled_header.source) );
            build.system( gcc, "gcc"..cppdirs..cppdefines..ccflags.." -o"..obj_directory(definition)..obj_name(target.precompiled_header.source).." "..target.precompiled_header.source, GccScanner );
        end        
    end
    
    local GccScanner = Scanner {
        [ [[((?:[A-Z]\:)?[^\:]+)\:([0-9]+)\:[0-9]+\: ([^\:]+)\:(.*)]] ] = function( filename, line, class, message )
            print( "%s(%s): %s: %s" % {filename, line, class, message} );
        end;
    };

    cppdefines = cppdefines.." -DBUILD_VERSION=\"\\\""..version.."\\\"\"";
    for dependency in target:get_dependencies() do
        if dependency:is_outdated() and dependency ~= target.precompiled_header then
            if dependency:prototype() == FilePrototype then
                print( leaf(dependency.source) );
                build.system( gcc, "gcc"..cppdirs..cppdefines..ccflags.." -o"..obj_directory(definition)..obj_name(dependency.source).." "..dependency.source, GccScanner );
            elseif dependency.results then
                for _, result in ipairs(dependency.results) do
                    if result:is_outdated() then
                        print( leaf(result.source) );
                        build.system( gcc, "gcc"..cppdirs..cppdefines..ccflags.." -o"..obj_directory(definition)..obj_name(result.source).." "..result.source, GccScanner );
                    end
                end
            end
        end    
    end
end

function mingw.build_library( target, definition )
    local ar = "%s/bin/ar.exe" % definition.settings.mingw.mingw_directory;

    local arflags = " ";
    
    local objects = "";
    for dependency in target:get_dependencies() do
        if dependency:prototype() == CompilePrototype then
            if dependency.precompiled_header ~= nil then
                objects = objects.." "..obj_name( dependency.precompiled_header:id() );
            end
            
            for object in dependency:get_dependencies() do
                if object:prototype() == FilePrototype and object ~= dependency.precompiled_header then
                    objects = objects.." "..obj_name( object:id() );
                end
            end
        end
    end
    
    if objects ~= "" then
        print( leaf(target:get_filename()) );
        pushd( obj_directory(target.module) );
        build.system( ar, "ar"..arflags.." -rcs "..native(target:get_filename())..objects );
        popd();
    end
end

function mingw.clean_library( target, definition )
    rm( target:get_filename() );
    rmdir( obj_directory(target.module) );
end

function mingw.build_executable( target, definition )
    local gxx = "%s/bin/g++.exe" % definition.settings.mingw.mingw_directory;

    local ldlibs = " ";
    
    local lddirs = " -L \""..definition.settings.lib.."\"";

    if definition.settings.library_directories then
        for _, directory in ipairs(definition.settings.library_directories) do
            lddirs = lddirs.." -L \""..directory.."\"";
        end
    end
    
    local ldflags = " ";

    ldflags = ldflags.." -o "..native( target:get_filename() );
    if target.module:prototype() == DynamicLibraryPrototype then
        ldflags = ldflags.." -shared -Wl,--out-implib,"..native( "%s/%s.lib" % {definition.settings.lib, basename(target:id())} );
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

    if definition.settings.generate_map_file then
        ldflags = ldflags.." -Wl,-Map,"..native(obj_directory(target.module)..target:id()..".map");
    end

    if definition.settings.stack_size then
        ldflags = ldflags.." -Wl,--stack,"..tostring(definition.settings.stack_size);
    end
    
    if definition.settings.strip then
        ldflags = ldflags.." -Wl,--strip-all";
    end

    local libraries = "";
    if target.libraries then
        for _, library in ipairs(target.libraries) do
            libraries = "%s -l%s" % { libraries, basename(library:id()) };
        end
    end
    if definition.third_party_libraries then
        for _, library in ipairs(definition.third_party_libraries) do
            libraries = "%s -l%s" % { libraries, library };
        end
    end

    local objects = "";
    for dependency in target:get_dependencies() do
        if dependency:prototype() == CompilePrototype then
            if dependency.precompiled_header ~= nil then
                objects = objects.." "..obj_name( dependency.precompiled_header:id() );
            end
            
            for object in dependency:get_dependencies() do
                if object:prototype() == FilePrototype and object ~= dependency.precompiled_header then
                    objects = objects.." "..obj_name( object:id() );
                end
            end
        end
    end

    if objects ~= "" then
        print( leaf(target:get_filename()) );
        pushd( obj_directory(target.module) );
        build.system( gxx, "g++"..ldflags..lddirs..objects..libraries..ldlibs );
        popd();
    end
end 

function mingw.clean_executable( target, definition )
    rm( target:get_filename() );
    rmdir( obj_directory(target.module) );
end

function mingw.obj_directory( target )
    return "%s/%s_%s/%s/" % { target.settings.obj, platform, variant, relative(target:directory(), root()) };
end

function mingw.cc_name( name )
    return "%s.c" % basename( name );
end

function mingw.cxx_name( name )
    return "%s.cpp" % basename( name );
end

function mingw.obj_name( name )
    return "%s.o" % basename( name );
end

function mingw.lib_name( name )
    return "%s_%s_%s.lib" % { name, platform, variant };
end

function mingw.exp_name( name )
    return "%s_%s_%s.exp" % { name, platform, variant };
end

function mingw.dll_name( name )
    return "%s_%s_%s.dll" % { name, platform, variant };
end

function mingw.exe_name( name )
    return "%s_%s_%s.exe" % { name, platform, variant };
end

function mingw.ilk_name( name )
    return "%s_%s_%s.ilk" % { name, platform, variant };
end
