
macosx = {};

function macosx.configure( settings )
    local function autodetect_macosx_sdk_version()
        local sdk_version = "";
        local sdk_build_version = "";

        local xcodebuild = "/usr/bin/xcodebuild";
        local arguments = "xcodebuild -sdk macosx -version";
        local result = build.execute( xcodebuild, arguments, nil, nil, function(line)
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
        local result = build.execute( xcodebuild, arguments, nil, nil, function(line)
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
        local result = build.execute( sw_vers, arguments, nil, nil, function(line)
            local version = line:match( "%w+" );
            if version then 
                os_version = version;
            end
        end );
        assert( result == 0, "Running sw_vers to extract operating system version failed" );

        return os_version;
    end

    if build.operating_system() == "macosx" then
        local local_settings = build.local_settings;
        if not local_settings.macosx then
            local_settings.updated = true;
            local_settings.macosx = {
                xcrun = "/usr/bin/xcrun";
                sdk_version = sdk_version;
                sdk_build_version = sdk_build_version;
                xcode_version = xcode_version;
                xcode_build_version = xcode_build_version;
                os_version = os_version;
            };
        end
    end
end

function macosx.initialize( settings )
    if build.platform_matches("macosx") then
        cc = macosx.cc;
        objc = macosx.objc;
        build_library = macosx.build_library;
        clean_library = macosx.clean_library;
        build_executable = macosx.build_executable;
        clean_executable = macosx.clean_executable;
        lipo_executable = macosx.lipo_executable;
        obj_directory = macosx.obj_directory;
        cc_name = macosx.cc_name;
        cxx_name = macosx.cxx_name;
        obj_name = macosx.obj_name;
        lib_name = macosx.lib_name;
        dll_name = macosx.dll_name;
        exe_name = macosx.exe_name;
    end
end

function macosx.cc( target )
    local flags = {
        '-DBUILD_OS_MACOSX'
    };
    clang.append_defines( target, flags );
    clang.append_include_directories( target, flags );
    clang.append_compile_flags( target, flags );
    
    local macosx_deployment_target = target.settings.macosx_deployment_target;
    if macosx_deployment_target then 
        table.insert( flags, ("-mmacosx-version-min=%s"):format(macosx_deployment_target) );
    end

    local ccflags = table.concat( flags, " " );
    local xcrun = target.settings.macosx.xcrun;

    for _, object in target:dependencies() do
        if object:outdated() then
            object:set_built( false );
            local source = object:dependency();
            print( build.leaf(source:id()) );
            local dependencies = ("%s.d"):format( object:filename() );
            local output = object:filename();
            local input = build.absolute( source:filename() );
            build.system( 
                xcrun, 
                ('xcrun --sdk macosx clang %s -MMD -MF "%s" -o "%s" "%s"'):format(ccflags, dependencies, output, input)
            );
            clang.parse_dependencies_file( dependencies, object );
            object:set_built( true );
        end
    end
end

function macosx.build_library( target )
    local flags = {
        "-static"
    };

    build.pushd( ("%s/%s"):format(obj_directory(target), target.architecture) );
    local objects =  {};
    for _, compile in target:dependencies() do
        local prototype = compile:prototype();
        if prototype == build.Cc or prototype == build.Cxx or prototype == build.ObjC or prototype == build.ObjCxx then
            for _, object in compile:dependencies() do
                table.insert( objects, build.relative(object:filename()) );
            end
        end
    end
    
    if #objects > 0 then
        local arflags = table.concat( flags, " " );
        local arobjects = table.concat( objects, '" "' );
        local xcrun = target.settings.macosx.xcrun;
        build.system( xcrun, ('xcrun --sdk macosx libtool %s -o "%s" "%s"'):format(arflags, build.native(target:filename()), arobjects) );
    end
    build.popd();
end

function macosx.clean_library( target )
    build.rm( target:filename() );
end

function macosx.build_executable( target )
    local flags = {};
    clang.append_link_flags( target, flags );

    local macosx_deployment_target = target.settings.macosx_deployment_target;
    if macosx_deployment_target then 
        table.insert( flags, ("-mmacosx-version-min=%s"):format(macosx_deployment_target) );
    end

    local rpaths = target.rpaths;
    if rpaths then 
        for _, rpath in ipairs(rpaths) do 
            table.insert( flags, ('-rpath "%s"'):format(rpath) );
        end
    end

    clang.append_library_directories( target, flags );

    local objects = {};
    local libraries = {};

    build.pushd( ("%s/%s"):format(obj_directory(target), target.architecture) );
    for _, dependency in target:dependencies() do
        local prototype = dependency:prototype();
        if prototype == build.Cc or prototype == build.Cxx or prototype == build.ObjC or prototype == build.ObjCxx then
            assertf( target.architecture == dependency.architecture, "Architectures for '%s' (%s) and '%s' (%s) don't match", target:path(), tostring(target.architecture), dependency:path(), tostring(dependency.architecture) );
            for _, object in dependency:dependencies() do
                if object:prototype() == nil then
                    table.insert( objects, build.relative(object:filename()) );
                end
            end
        elseif prototype == build.StaticLibrary or prototype == build.DynamicLibrary then
            table.insert( libraries, ("-l%s"):format(dependency:id()) );
        end
    end

    clang.append_link_libraries( target, libraries );

    if #objects > 0 then
        local ldflags = table.concat( flags, " " );
        local ldobjects = table.concat( objects, '" "' );
        local ldlibs = table.concat( libraries, " " );
        local xcrun = target.settings.macosx.xcrun;
        build.system( xcrun, ('xcrun --sdk macosx clang++ %s "%s" %s'):format(ldflags, ldobjects, ldlibs) );
    end
    build.popd();
end

function macosx.clean_executable( target )
    build.rm( target:filename() );
end

function macosx.lipo_executable( target )
    local executables = {};
    for _, executable in target:dependencies() do 
        table.insert( executables, executable:filename() );
    end
    executables = table.concat( executables, [[" "]] );
    local xcrun = target.settings.macosx.xcrun;
    build.system( xcrun, ('xcrun --sdk macosx lipo -create -output "%s" "%s"'):format(target:filename(), executables) );
end

function macosx.obj_directory( target )
    return ("%s/%s"):format( target.settings.obj, build.relative(target:working_directory():path(), build.root()) );
end

function macosx.cc_name( name )
    return ("%s.c"):format( build.basename(name) );
end

function macosx.cxx_name( name )
    return ("%s.cpp"):format( build.basename(name) );
end

function macosx.obj_name( name, architecture )
    return ("%s.o"):format( build.basename(name) );
end

function macosx.lib_name( name )
    return ("lib%s.a"):format( name );
end

function macosx.dll_name( name )
    return ("%s.dylib"):format( name );
end

function macosx.exe_name( name, architecture )
    return name;
end

build.register_module( macosx );
