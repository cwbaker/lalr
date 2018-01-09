
android = {};

local directory_by_architecture = {
    ["armv5"] = "armeabi";
    ["armv7"] = "armeabi-v7a";
    ["mips"] = "mips";
    ["x86"] = "x86";
};

function android.configure( settings )
    function autodetect_ndk_directory()
        if build.operating_system() == "windows" then
            return "C:/android/android-ndk";
        else
            return build.home( "Library/Android/ndk" );
        end
    end

    function autodetect_sdk_directory()
        if build.operating_system() == "windows" then
            return "C:/Program Files (x86)/Android/android-sdk";
        else
            return build.home( "Library/Android/sdk" );
        end
    end

    local local_settings = build.local_settings;
    if not local_settings.android then
        local_settings.updated = true;
        local_settings.android = {
            ndk_directory = autodetect_ndk_directory();
            sdk_directory = autodetect_sdk_directory();
            build_tools_directory = ("%s/build-tools/22.0.1"):format( autodetect_sdk_directory() );
            toolchain_version = "4.9";
            ndk_platform = "android-19";
            sdk_platform = "android-22";
            architectures = { "armv5", "armv7" };
        };
    end
end

function android.toolchain_directory( settings, architecture )
    local android = settings.android;
    local toolchain_by_architecture = {
        ["armv5"] = "arm-linux-androideabi",
        ["armv7"] = "arm-linux-androideabi",
        ["mips"] = "mipsel-linux-android",
        ["x86"] = "x86"
    };
    local prebuilt_by_operating_system = {
        windows = "windows";
        macosx = "darwin-x86_64";
    };
    return ("%s/toolchains/%s-%s/prebuilt/%s"):format( 
        android.ndk_directory, 
        toolchain_by_architecture [architecture], 
        android.toolchain_version, 
        prebuilt_by_operating_system [build.operating_system()]
    );
end

function android.platform_directory( settings, architecture )
    local android = settings.android;
    local arch_by_architecture = {
        ["armv5"] = "arm",
        ["armv7"] = "arm",
        ["mips"] = "mips",
        ["x86"] = "x86"
    };
    return ("%s/platforms/%s/arch-%s"):format( android.ndk_directory, android.ndk_platform, arch_by_architecture[architecture] );
end

function android.include_directories( settings, architecture )
    local android = settings.android;
    local runtime_library = settings.runtime_library;
    if runtime_library:match("gnustl.*") then
        return {
            ("%s/sources/cxx-stl/gnu-libstdc++/%s/libs/%s/include"):format( android.ndk_directory, android.toolchain_version, directory_by_architecture[architecture] ),
            ("%s/sources/cxx-stl/gnu-libstdc++/%s/include"):format( android.ndk_directory, android.toolchain_version )
        };
    elseif runtime_library:match("stlport.*") then
        return {
            ("%s/sources/cxx-stl/stlport/stlport"):format( android.ndk_directory )
        };
    elseif runtime_library:match("c++.*") then
        return {
            ("%s/sources/cxx-stl/llvm-libc++/libcxx/include"):format( android.ndk_directory )
        };
    elseif runtime_library:match("gabi++.*") then 
        return {
            ("%s/sources/cxx-stl/gabi++/include"):format( android.ndk_directory )
        };
    else 
        assertf( false, "Unrecognized C++ runtime library '%s'", tostring(runtime_library) );
    end
end

function android.library_directories( settings, architecture )
    local runtime_library = settings.runtime_library;
    if runtime_library:match("gnustl.*") then
        return {
            ("%s/usr/lib"):format( android.platform_directory(settings, architecture) ),
            ("%s/sources/cxx-stl/gnu-libstdc++/%s/libs/%s"):format( settings.android.ndk_directory, settings.android.toolchain_version, directory_by_architecture[architecture] )
        };
    elseif runtime_library:match("stlport.*") then
        return {
            ("%s/usr/lib"):format( android.platform_directory(settings, architecture) ),
            ("%s/sources/cxx-stl/stlport/libs/%s"):format( settings.android.ndk_directory, directory_by_architecture[architecture] )
        };
    elseif runtime_library:match("c++.*") then 
        return {
            ("%s/usr/lib"):format( android.platform_directory(settings, architecture) ),
            ("%s/sources/cxx-stl/llvm-libc++/libs/%s"):format( settings.android.ndk_directory, directory_by_architecture[architecture] )
        };
    elseif runtime_library:match("gabi++.*") then 
        return {
            ("%s/usr/lib"):format( android.platform_directory(settings, architecture) ),
            ("%s/sources/cxx-stl/gabi++/libs/%s"):format( settings.android.ndk_directory, directory_by_architecture[architecture] )
        };
    else 
        assertf( false, "Unrecognized C++ runtime library '%s'", tostring(runtime_library) );
    end
end

function android.initialize( settings )
    if build.platform_matches("android") then
        if build.operating_system() == "windows" then
            local path = {
                ("%s/bin"):format( android.toolchain_directory(settings, "armv5") )
            };
            android.environment = {
                PATH = table.concat( path, ";" );
            };
        else
            local path = {
                "/usr/bin",
                "/bin",
                ("%s/bin"):format( android.toolchain_directory(settings, "armv5") )
            };
            android.environment = {
                PATH = table.concat( path, ":" );
            };
        end

        settings.android.proguard_enabled = settings.android.proguard_enabled or variant == "shipping";
        
        cc = android.cc;
        build_library = android.build_library;
        clean_library = android.clean_library;
        build_executable = android.build_executable;
        clean_executable = android.clean_executable;
        obj_directory = android.obj_directory;
        cc_name = android.cc_name;
        cxx_name = android.cxx_name;
        pch_name = android.pch_name;
        pdb_name = android.pdb_name;
        obj_name = android.obj_name;
        lib_name = android.lib_name;
        exp_name = android.exp_name;
        dll_name = android.dll_name;
        exe_name = android.exe_name;        
        ilk_name = android.ilk_name;
    end
end

function android.cc( target )
    local flags = {
        "-DBUILD_OS_ANDROID";
        "-D__ARM_ARCH_5__",
        "-D__ARM_ARCH_5T__",
        "-D__ARM_ARCH_5E__",
        "-D__ARM_ARCH_5TE__",
        "-DANDROID"
    };
    gcc.append_defines( target, flags );
    gcc.append_version_defines( target, flags );

    table.insert( flags, ("--sysroot=%s"):format(android.platform_directory(target.settings, target.architecture)) );

    gcc.append_include_directories( target, flags );
    for _, directory in ipairs(android.include_directories(target.settings, target.architecture)) do
        assert( build.exists(directory), ("The include directory '%s' does not exist"):format(directory) );
        table.insert( flags, ([[-I"%s"]]):format(directory) );
    end

    gcc.append_compile_flags( target, flags );

    local ccflags = table.concat( flags, " " );
    local gcc_ = ("%s/bin/arm-linux-androideabi-gcc"):format( android.toolchain_directory(target.settings, target.architecture) );
    for _, object in target:dependencies() do
        if object:outdated() then
            object:set_built( false );
            local source = object:dependency();
            print( build.leaf(source:id()) );
            local output = object:filename();
            local input = build.relative( source:filename() );
            build.system( 
                gcc_, 
                ('arm-linux-androideabi-gcc %s -o "%s" "%s"'):format(ccflags, output, input), 
                android.environment,
                build.dependencies_filter(object)
            );
            object:set_built( true );
        end
    end
end

function android.build_library( target )
    local flags = {
        "-rcs"
    };
    
    build.pushd( ("%s/%s"):format(obj_directory(target), target.architecture) );
    local objects = {};
    for _, compile in target:dependencies() do
        local prototype = compile:prototype();
        if prototype == build.Cc or prototype == build.Cxx then
            for _, object in compile:dependencies() do
                table.insert( objects, build.relative(object:filename()) )
            end
        end
    end
    
    if #objects > 0 then
        print( build.leaf(target:filename()) );
        local arflags = table.concat( flags, " " );
        local arobjects = table.concat( objects, '" "' );
        local ar = ("%s/bin/arm-linux-androideabi-ar"):format( android.toolchain_directory(target.settings, target.architecture) );
        build.system( ar, ('ar %s "%s" "%s"'):format(arflags, build.native(target:filename()), arobjects), android.environment );
    end
    build.popd();
end

function android.clean_library( target )
    build.rm( target:filename() );
    build.rmdir( obj_directory(target) );
end

function android.build_executable( target )
    local flags = { 
        ("--sysroot=%s"):format( android.platform_directory(target.settings, target.architecture) ),
        ("-Wl,-soname,%s"):format( build.leaf(target:filename()) ),
        "-shared",
        "-no-canonical-prefixes",
        "-Wl,--no-undefined",
        "-Wl,-z,noexecstack",
        "-Wl,-z,relro",
        "-Wl,-z,now",
        ('-o "%s"'):format( build.native(target:filename()) )
    };

    gcc.append_link_flags( target, flags );
    gcc.append_library_directories( target, flags );
    for _, directory in ipairs(android.library_directories(target.settings, target.architecture)) do
        table.insert( flags, ('-L"%s"'):format(directory) );
    end

    local objects = {};
    local libraries = {};

    build.pushd( ("%s/%s"):format(obj_directory(target), target.architecture) );
    for _, dependency in target:dependencies() do
        local prototype = dependency:prototype();
        if prototype == build.Cc or prototype == build.Cxx then
            for _, object in dependency:dependencies() do
                if object:prototype() == nil then
                    table.insert( objects, build.relative(object:filename()) );
                end
            end
        elseif prototype == build.StaticLibrary or prototype == build.DynamicLibrary then
            if dependency.whole_archive then
                table.insert( libraries, ("-Wl,--whole-archive") );
            end
            table.insert( libraries, ("-l%s"):format(dependency:id()) );
            if dependency.whole_archive then
                table.insert( libraries, ("-Wl,--no-whole-archive") );
            end
        end
    end

    gcc.append_link_libraries( target, libraries );
    local runtime_library = target.settings.runtime_library;
    if runtime_library then 
        table.insert( flags, ("-l%s"):format(runtime_library) );
    end

    if #objects > 0 then
        local ldflags = table.concat( flags, " " );
        local ldobjects = table.concat( objects, '" "' );
        local ldlibs = table.concat( libraries, " " );
        local gxx = ("%s/bin/arm-linux-androideabi-g++"):format( android.toolchain_directory(target.settings, target.architecture) );

        print( build.leaf(target:filename()) );
        build.system( gxx, ('arm-linux-androideabi-g++ %s "%s" %s'):format(ldflags, ldobjects, ldlibs), android.environment );
    end
    build.popd();
end 

function android.clean_executable( target )
    build.rm( target:filename() );
    build.rmdir( obj_directory(target) );
end

-- Deploy the fist Android .apk package found in the dependencies of the 
-- current working directory.
function android.deploy( directory )
    local sdk_directory = build.settings.android.sdk_directory;
    if sdk_directory then 
        local directory = directory or build.find_target( build.initial() );
        local apk = nil;
        for _, dependency in directory:dependencies() do
            if dependency:prototype() == android.Apk then 
                apk = dependency;
                break;
            end
        end
        assertf( apk, "No android.Apk target found as a dependency of '%s'", directory:path() );
        local adb = ("%s/platform-tools/adb"):format( sdk_directory );
        assertf( build.is_file(adb), "No 'adb' executable found at '%s'", adb );

        local device_connected = false;
        local function adb_get_state_filter( state )
            device_connected = string.find( state, "device" ) ~= nil;
        end
        build.system( adb, ('adb get-state'), android.environment, nil, adb_get_state_filter );
        if device_connected then
            printf( "Deploying '%s'...", apk:filename() );
            build.system( adb, ('adb install -r "%s"'):format(apk:filename()), android.environment );
        end
    end
end

function android.obj_directory( target )
    return ("%s/%s"):format( target.settings.obj, build.relative(target:working_directory():path(), build.root()) );
end

function android.cc_name( name )
    return ("%s.c"):format( build.basename(name) );
end

function android.cxx_name( name )
    return ("%s.cpp"):format( build.basename(name) );
end

function android.obj_name( name, architecture )
    return ("%s.o"):format( build.basename(name) );
end

function android.lib_name( name )
    return ("lib%s.a"):format( name );
end

function android.dll_name( name, architecture )
    return ("lib/%s/lib%s.so"):format( directory_by_architecture[architecture], build.basename(name) );
end

function android.exe_name( name, architecture )
    return ("%s_%s_%s_%s"):format( name, architecture, platform, variant );
end

function android.android_jar( settings )
    local settings = settings or build.current_settings();
    return ("%s/platforms/%s/android.jar"):format( settings.android.sdk_directory, settings.android.sdk_platform );
end

function android.DynamicLibrary( name, architecture )
    local filename = ("${apk}/%s"):format( android.dll_name(name, architecture) );
    local dynamic_library = build.default_create( build.DynamicLibrary, filename );
    dynamic_library.architecture = architecture or settings.default_architecture;

    local group = build.Target( build.anonymous() );
    group:add_dependency( dynamic_library );
    group.depend = function( group, dependencies )
        return dynamic_library:depend( dependencies );
    end

    local runtime_library = dynamic_library.settings.runtime_library;
    if runtime_library then 
        if runtime_library:match(".*_shared") then 
            local destination = ("%s/lib%s.so"):format( build.branch(dynamic_library:filename()), runtime_library );
            for _, directory in ipairs(android.library_directories(dynamic_library.settings, dynamic_library.architecture)) do
                local source = ("%s/lib%s.so"):format( directory, runtime_library );
                if build.exists(source) then
                    local copy = build.Copy (destination) (source);
                    group:add_dependency( copy );
                    break;
                end
            end
        end
    end

    return group;
end

require "build.android.Aidl";
require "build.android.Apk";
require "build.android.BuildConfig";
require "build.android.Dex";
require "build.android.R";

build.register_module( android );
