
-- Switch platform to 'ios_simulator' if the specified platform is 'ios' but
-- the environment variable 'SDKROOT' contains 'iPhoneSimulator'.  This is to
-- accomodate builds triggered from Xcode that always specify 'ios' as the 
-- platform but might be building for the iOS simulator.
local sdkroot = os.getenv( "SDKROOT" );
if platform and platform == "ios" and sdkroot and sdkroot:find("iPhoneSimulator") then 
    platform = "ios_simulator";
end

ios = {};

function ios.sdkroot_by_target_and_platform( target, platform )
    local sdkroot = target.settings.sdkroot or "iphoneos";
    if platform == "ios_simulator" then 
        sdkroot = string.gsub( sdkroot, "iphoneos", "iphonesimulator" );
    end
    return sdkroot;
end

function ios.configure( settings )
    local function autodetect_iphoneos_sdk_version()
        local sdk_version = "";
        local sdk_build_version = "";

        local xcodebuild = "/usr/bin/xcodebuild";
        local arguments = "xcodebuild -sdk iphoneos -version";
        local result = build:execute( xcodebuild, arguments, nil, nil, function(line)
            local key, value = line:match( "(%w+): ([^\n]+)" );
            if key and value then 
                if key == "ProductBuildVersion" then 
                    sdk_build_version = value;
                elseif key == "SDKVersion" then
                    sdk_version = value;
                end
            end
        end );
        assert( result == 0, "Running xcodebuild to extract SDK name and version failed" );

        return sdk_version, sdk_build_version;
    end

    local function autodetect_xcode_version()
        local xcode_version = "";
        local xcode_build_version = "";

        local xcodebuild = "/usr/bin/xcodebuild";
        local arguments = "xcodebuild -version";
        local result = build:execute( xcodebuild, arguments, nil, nil, function(line)
            local major, minor = line:match( "Xcode (%d+)%.(%d+)" );
            if major and minor then 
                xcode_version = ("%02d%02d"):format( tonumber(major), tonumber(minor) );
            end

            local build_version = line:match( "Build version (%w+)" )
            if build_version then
                xcode_build_version = build_version;
            end
        end );
        assert( result == 0, "Running xcodebuild to extract Xcode version failed" );
        
        return xcode_version, xcode_build_version;
    end

    local function autodetect_macosx_version()
        local os_version = "";

        local sw_vers = "/usr/bin/sw_vers";
        local arguments = "sw_vers -buildVersion";
        local result = build:execute( sw_vers, arguments, nil, nil, function(line)
            local version = line:match( "%w+" );
            if version then 
                os_version = version;
            end
        end );
        assert( result == 0, "Running sw_vers to extract operating system version failed" );

        return os_version;
    end

    local local_settings = build.local_settings;
    if not local_settings.ios then
        local sdk_version, sdk_build_version = autodetect_iphoneos_sdk_version();
        local xcode_version, xcode_build_version = autodetect_xcode_version();
        local os_version = autodetect_macosx_version();
        local_settings.updated = true;
        local_settings.ios = {
            xcrun = "/usr/bin/xcrun";
            signing_identity = "iPhone Developer";
            codesign = "/usr/bin/codesign";
            plutil = "/usr/bin/plutil";
            sdk_name = "iphoneos";
            sdk_version = sdk_version;
            sdk_build_version = sdk_build_version;
            xcode_version = xcode_version;
            xcode_build_version = xcode_build_version;
            os_version = os_version;
            architectures = { "armv7", "arm64" };
        };
    end
end;

function ios.initialize( settings )
    for _, architecture in ipairs(settings.ios.architectures) do 
        build:default_build( ("ios-%s"):format(architecture), build:configure {
            platform = "ios";
            architecture = architecture;
            default_architecture = architecture;
            cc = ios.cc;
            objc = ios.objc;
            build_library = ios.build_library;
            clean_library = ios.clean_library;
            build_executable = ios.build_executable;
            clean_executable = ios.clean_executable;
            lipo_executable = ios.lipo_executable;
            obj_directory = ios.obj_directory;
            cc_name = ios.cc_name;
            cxx_name = ios.cxx_name;
            obj_name = ios.obj_name;
            lib_name = ios.lib_name;
            dll_name = ios.dll_name;
            exe_name = ios.exe_name;
        } );
    end
end;

function ios.cc( target )
    local flags = {
        '-DBUILD_OS_IOS'
    };
    clang.append_defines( target, flags );
    clang.append_include_directories( target, flags );
    clang.append_compile_flags( target, flags );

    local settings = target.settings;
    local iphoneos_deployment_target = settings.iphoneos_deployment_target;
    if iphoneos_deployment_target then 
        table.insert( flags, ("-miphoneos-version-min=%s"):format(iphoneos_deployment_target) );
    end

    local sdkroot = ios.sdkroot_by_target_and_platform( target, settings.platform );
    local ccflags = table.concat( flags, " " );
    local xcrun = settings.ios.xcrun;

    for _, object in target:dependencies() do
        if object:outdated() then
            object:set_built( false );
            local source = object:dependency();
            print( build:leaf(source) );
            local dependencies = ("%s.d"):format( object );
            local output = object:filename();
            local input = build:absolute( source );
            build:system( 
                xcrun, 
                ('xcrun --sdk %s clang %s -MMD -MF "%s" -o "%s" "%s"'):format(sdkroot, ccflags, dependencies, output, input)
            );
            clang.parse_dependencies_file( dependencies, object );
            object:set_built( true );
        end
    end
end;

function ios.build_library( target )
    local flags = {
        "-static"
    };

    local settings = target.settings;
    build:pushd( ("%s/%s_%s"):format(settings.obj_directory(target), settings.platform, settings.architecture) );
    local objects =  {};
    for _, dependency in target:dependencies() do
        local prototype = dependency:prototype();
        if prototype == build.Cc or prototype == build.Cxx or prototype == build.ObjC or prototype == build.ObjCxx then
            for _, object in dependency:dependencies() do
                table.insert( objects, build:relative(object:filename()) );
            end
        end
    end
    
    if #objects > 0 then
        local sdkroot = ios.sdkroot_by_target_and_platform( target, settings.platform );
        local arflags = table.concat( flags, " " );
        local arobjects = table.concat( objects, [[" "]] );
        local xcrun = target.settings.ios.xcrun;
        build:system( xcrun, ('xcrun --sdk %s libtool %s -o "%s" "%s"'):format(sdkroot, arflags, build:native(target:filename()), arobjects) );
    end
    build:popd();
end;

function ios.clean_library( target )
    build:rm( target:filename() );
    build:rmdir( obj_directory(target) );
end;

function ios.build_executable( target )
    local flags = {};
    clang.append_link_flags( target, flags );
    table.insert( flags, "-ObjC" );
    table.insert( flags, "-all_load" );

    local settings = target.settings;
    local iphoneos_deployment_target = settings.iphoneos_deployment_target;
    if iphoneos_deployment_target then 
        if settings.platform == "ios" then 
            table.insert( flags, ("-mios-version-min=%s"):format(iphoneos_deployment_target) );
        elseif settings.platform == "ios_simulator" then
            table.insert( flags, ("-mios-simulator-version-min=%s"):format(iphoneos_deployment_target) );
        end
    end

    clang.append_library_directories( target, flags );

    local objects = {};
    local libraries = {};

    build:pushd( ("%s/%s_%s"):format(settings.obj_directory(target), settings.platform, settings.architecture) );
    for _, dependency in target:dependencies() do
        local prototype = dependency:prototype();
        if prototype == build.Cc or prototype == build.Cxx or prototype == build.ObjC or prototype == build.ObjCxx then
            for _, object in dependency:dependencies() do
                table.insert( objects, build:relative(object:filename()) );
            end
        elseif prototype == build.StaticLibrary or prototype == build.DynamicLibrary then
            table.insert( libraries, ("-l%s"):format(dependency:id()) );
        end
    end

    clang.append_link_libraries( target, libraries );

    if #objects > 0 then
        local sdkroot = ios.sdkroot_by_target_and_platform( target, settings.platform );
        local ldflags = table.concat( flags, " " );
        local ldobjects = table.concat( objects, '" "' );
        local ldlibs = table.concat( libraries, " " );
        local xcrun = settings.ios.xcrun;
        build:system( xcrun, ('xcrun --sdk %s clang++ %s "%s" %s'):format(sdkroot, ldflags, ldobjects, ldlibs) );
    end
    build:popd();
end

function ios.clean_executable( target )
    build:rm( target:filename() );
    build:rmdir( obj_directory(target) );
end

function ios.lipo_executable( target )
    local executables = {};
    for _, executable in target:dependencies() do 
        table.insert( executables, executable:filename() );
    end
    local settings = target.settings;
    local sdk = ios.sdkroot_by_target_and_platform( target, settings.platform );
    executables = table.concat( executables, '" "' );
    local xcrun = target.settings.ios.xcrun;
    build:system( xcrun, ('xcrun --sdk %s lipo -create -output "%s" "%s"'):format(sdk, target:filename(), executables) );
end

-- Deploy the fist iOS .app bundle found in the dependencies of the current
-- working directory.
function ios.deploy( directory )
    local ios_deploy = build.settings.ios.ios_deploy;
    if ios_deploy then 
        local directory = directory or find_target( initial("all") );
        local app = nil;
        for _, dependency in directory:dependencies() do
            if dependency:prototype() == ios.App then 
                app = dependency;
                break;
            end
        end
        assertf( app, "No ios.App target found as a dependency of '%s'", directory:path() );
        assertf( build:is_file(ios_deploy), "No 'ios-deploy' executable found at '%s'", ios_deploy );
        build:system( ios_deploy, ('ios-deploy --timeout 1 --bundle "%s"'):format(app:filename()) );
    else
        printf( "No 'ios-deploy' executable specified by 'settings.ios.ios_deploy'" );
    end
end

function ios.obj_directory( target )
    return ("%s/%s"):format( target.settings.obj, build:relative(target:working_directory():path(), build:root()) );
end

function ios.cc_name( name )
    return ("%s.c"):format( build:basename(name) );
end

function ios.cxx_name( name )
    return ("%s.cpp"):format( build:basename(name) );
end

function ios.obj_name( name, architecture )
    return ("%s.o"):format( build:basename(name) );
end

function ios.lib_name( name )
    return ("lib%s.a"):format( name );
end

function ios.dll_name( name )
    return ("%s.dylib"):format( name );
end

function ios.exe_name( name )
    return ("%s"):format( name );
end

require "build.ios.App";

build:register_module( ios );
