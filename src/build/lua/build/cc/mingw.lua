
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
            ("%s/bin"):format( mingw_directory ),
            os.getenv("PATH")
        };
        putenv( "PATH", table.concat(path, ";") );
        
        local include = {
            ("%s/include"):format( mingw_directory ),
            os.getenv("INCLUDE")
        };
        putenv( "INCLUDE", table.concat(include, ";") );
        
        local lib = {
            ("%s/lib"):format( mingw_directory ),
            os.getenv("LIB")
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

function mingw.cc( target )
    local defines = {
        ("-DBUILD_OS_%s"):format( upper(operating_system()) ),
        ("-DBUILD_PLATFORM_%s"):format( upper(platform) ),
        ("-DBUILD_VARIANT_%s"):format( upper(variant) ),
        ("-DBUILD_LIBRARY_SUFFIX=\"\\\"_%s_%s.lib\\\"\""):format( platform, variant ),
        ("-DBUILD_LIBRARY_TYPE_%s"):format( upper(target.settings.library_type) ),
        "-D_WIN32"
    };

    if target.settings.debug then
        table.insert( defines, "-D_DEBUG" );
        table.insert( defines, "-DDEBUG" );
    else
        table.insert( defines, "-DNDEBUG" );
    end

    if target.settings.defines then
        for _, define in ipairs(target.settings.defines) do
            table.insert( defines, ("-D%s"):format(define) );
        end
    end    
    if target.defines then
        for _, define in ipairs(target.defines) do
            table.insert( defines, ("-D%s"):format(define) );
        end
    end

    local include_directories = {};
    if target.include_directories then
        for _, directory in ipairs(target.include_directories) do
            table.insert( include_directories, ('-I"%s"'):format(relative(directory)) );
        end
    end
    if target.settings.include_directories then
        for _, directory in ipairs(target.settings.include_directories) do
            table.insert( include_directories, ('-I"%s"'):format(directory) );
        end
    end

    local flags = {
        "-c",
    };
    
    local language = target.language or "c++";
    if language then
        table.insert( flags, ("-x %s"):format(language) );
        if string.find(language, "c++", 1, true) then
            table.insert( flags, "-std=c++11" );
            table.insert( flags, "-Wno-deprecated" );
            table.insert( flags, "-fpermissive" );
            if target.settings.runtime_library == "static" or target.settings.runtime_library == "static_debug" then
                table.insert( flags, "-static" );
            end
            if target.settings.exceptions then
                table.insert( flags, "-fexceptions" );
            end
            if target.settings.run_time_type_info then
                table.insert( flags, "-frtti" );
            end
        end
    end
    
    if target.settings.debug then
        table.insert( flags, "-g" );
    end

    if target.settings.optimization then
        table.insert( flags, "-O3" );
        table.insert( flags, "-Ofast" );
    end
    
    if target.settings.preprocess then
        table.insert( flags, "-E" );
    end

    local gcc = ("%s/bin/gcc.exe"):format( target.settings.mingw.mingw_directory );
    local cppdefines = table.concat( defines, " " );
    local cppdirs = table.concat( include_directories, " " );
    local ccflags = table.concat( flags, " " );

    local GccScanner = Scanner {
        [ [[((?:[A-Z]\:)?[^\:]+)\:([0-9]+)\:[0-9]+\: ([^\:]+)\:(.*)]] ] = function( filename, line, class, message )
            print( ("%s(%s): %s: %s"):format(filename, line, class, message) );
        end;
    };

    table.insert( defines, ('-DBUILD_VERSION="\\"%s\\""'):format(version) );
    cppdefines = table.concat( defines, " " );

    for _, dependency in target:dependencies() do
        if dependency:is_outdated() and dependency ~= target.precompiled_header then
            if dependency:prototype() == nil then
                print( leaf(dependency.source) );
                build:system( gcc, ([[gcc %s %s %s -o "%s" "%s"]]):format(cppdirs, cppdefines, ccflags, relative(dependency:filename()), dependency.source), GccScanner );
            elseif dependency.results then
                for _, result in ipairs(dependency.results) do
                    if result:is_outdated() then
                        print( leaf(result.source) );
                        build:system( gcc, ([[gcc %s %s %s -o "%s" "%s"]]):format(cppdirs, cppdefines, ccflags, relative(dependency:filename()), result.source), GccScanner );
                    end
                end
            end
        end    
    end
end

function mingw.build_library( target )
    local flags = {
        "-rcs"
    };
    
    pushd( ("%s/%s"):format(obj_directory(target), target.architecture) );
    local objects = {};
    for _, dependency in target:dependencies() do
        local prototype = dependency:prototype();
        if prototype == Cc or prototype == Cxx then
            for _, object in dependency:dependencies() do
                if object:prototype() == nil and object ~= dependency.precompiled_header then
                    table.insert( objects, relative(object:filename()) );
                end
            end
        end
    end
    
    if #objects > 0 then
        print( leaf(target:filename()) );
        local ar = ("%s/bin/ar.exe"):format( target.settings.mingw.mingw_directory );
        local arflags = table.concat( flags, " " );
        local arobjects = table.concat( objects, '" "' );
        build:system( ar, ('ar %s "%s" "%s"'):format(arflags, native(target:filename()), arobjects) );
    end
    popd();
end

function mingw.clean_library( target )
    rm( target:filename() );
    rmdir( obj_directory(target) );
end

function mingw.build_executable( target )
    local flags = {
        ('-o "%s"'):format( native(target:filename()) ),
    };

    if target:prototype() == DynamicLibrary then
        table.insert( flags, "-shared" );
        table.insert( flags, ('-Wl,--out-implib,"%s"'):format(native((("%s/%s")):format(target.settings.lib, lib_name(target:id())))) );
    end
    
    if target.settings.verbose_linking then
        table.insert( flags, "-Wl,--verbose=31" );
    end
    
    if target.settings.runtime_library == "static" or target.settings.runtime_library == "static_debug" then
        table.insert( flags, "-static" );
    end
    
    if target.settings.debug then
        table.insert( flags, "-debug" );
    end

    if target.settings.generate_map_file then
        table.insert( flags, ('-Wl,-Map,"%s"'):format(native(("%s/%s.map"):format(obj_directory(target), target:id()))) );
    end

    if target.settings.stack_size then
        table.insert( flags, ("-Wl,--stack,%d"):format(target.settings.stack_size) );
    end
    
    if target.settings.strip then
        table.insert( flags, "-Wl,--strip-all" );
    end

    local library_directories = {};
    if target.library_directories then
        for _, directory in ipairs(target.settings.library_directories) do
            table.insert( library_directories, ('-L "%s"'):format(directory) );
        end
    end
    if target.settings.library_directories then
        for _, directory in ipairs(target.settings.library_directories) do
            table.insert( library_directories, ('-L "%s"'):format(directory) );
        end
    end    


    pushd( ("%s/%s"):format(obj_directory(target), target.architecture) );
    local objects = {};
    local libraries = {};

    for _, dependency in target:dependencies() do
        local prototype = dependency:prototype();
        if prototype == Cc or prototype == Cxx then
            for _, object in dependency:dependencies() do
                if object:prototype() == nil and object ~= dependency.precompiled_header then
                    table.insert( objects, relative(object:filename()) );
                end
            end
        elseif prototype == StaticLibrary or prototype == DynamicLibrary then
            table.insert( libraries, ("-l%s"):format(dependency:id()) );
        end
    end

    if target.settings.third_party_libraries then
        for _, library in ipairs(target.settings.third_party_libraries) do
            table.insert( libraries, ("-l%s"):format(library) );
        end
    end
    
    if target.third_party_libraries then
        for _, library in ipairs(target.third_party_libraries) do
            table.insert( libraries, ("-l%s"):format(library) );
        end
    end

    if #objects > 0 then
        local gxx = ("%s/bin/g++.exe"):format( target.settings.mingw.mingw_directory );
        local ldflags = table.concat( flags, " " );
        local lddirs = table.concat( library_directories, " " );        
        local ldobjects = table.concat( objects, '" "' );
        local ldlibs = table.concat( libraries, " " );
        print( leaf(target:filename()) );
        build:system( gxx, ('g++ %s %s "%s" %s'):format(ldflags, lddirs, ldobjects, ldlibs) );
    end
    popd();
end 

function mingw.clean_executable( target )
    rm( target:filename() );
    rmdir( obj_directory(target) );
end

function mingw.obj_directory( target )
    return ("%s/%s_%s/%s/"):format( target.settings.obj, platform, variant, relative(target:working_directory():path(), root()) );
end

function mingw.cc_name( name )
    return ("%s.c"):format( basename(name) );
end

function mingw.cxx_name( name )
    return ("%s.cpp"):format( basename(name) );
end

function mingw.obj_name( name )
    return ("%s.o"):format( basename(name) );
end

function mingw.lib_name( name, architecture )
    return ("lib%s_%s.a"):format( name, architecture );
end

function mingw.exp_name( name )
    return ("%s_%s.exp"):format( name, architecture );
end

function mingw.dll_name( name, architecture )
    return ("%s_%s.dll"):format( name, architecture );
end

function mingw.exe_name( name, architecture )
    return ("%s_%s.exe"):format( name, architecture );
end

function mingw.ilk_name( name )
    return ("%s_%s.ilk"):format( name, architecture );
end

build:register_module( mingw );
