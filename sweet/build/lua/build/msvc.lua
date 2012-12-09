
msvc = {};

function msvc.configure( settings )
    local function registry( key )
        local values = {};
        local RegQueryScanner = Scanner {
            [ [[[ ]* ([A-Za-z0-9_]+) [ ]* ([A-Za-z0-9_]+) [ ]* ([A-Za-z0-9_\\\:\. ]+)]] ] = function( key, type, value )
                values[key] = value;
            end;
            
            [ [[.*]] ] = function()
            end;
        };
        local reg = "C:/Windows/system32/reg.exe";
        local arguments = [[reg query "%s"]] % key;
        build.system( reg, arguments, RegQueryScanner );
        return values;
    end

    local function autodetect_visual_studio_directory()
        local visual_studio_directory = os.getenv( "VS90COMNTOOLS" ) or os.getenv( "VS100COMNTOOLS" );
        if visual_studio_directory then
            visual_studio_directory = string.gsub( visual_studio_directory, "\\Common7\\Tools\\", "" );
        end    
        return visual_studio_directory;
    end

    local function autodetect_windows_sdk_directory()
        local windows_sdk = registry( [[HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows]] );
        assert( windows_sdk.CurrentInstallFolder, "Windows SDK not found!" );
        return windows_sdk.CurrentInstallFolder;
    end

    local local_settings = build.local_settings;
    if not local_settings.msvc then
        local_settings.updated = true;
        local_settings.msvc = {
            visual_studio_directory = autodetect_visual_studio_directory() or "C:/Program Files/Microsoft Visual Studio 9.0";
            windows_sdk_directory = autodetect_windows_sdk_directory() or "C:/Program Files/Microsoft SDKs/Windows/v6.0A";
        };
    end
end;

function msvc.initialize( settings )
    msvc.configure( settings );
    if platform == "msvc" then
        -- Make sure that the environment variable VS_UNICODE_OUTPUT is not set.  
        -- Visual Studio sets this to signal its tools to communicate back to 
        -- Visual Studio using named pipes rather than stdout so that unicode output 
        -- works better but this then prevents the build tool from intercepting
        -- and collating this output.
        -- See http://blogs.msdn.com/freik/archive/2006/04/05/569025.aspx.
        putenv( "VS_UNICODE_OUTPUT", "" );

        local visual_studio_directory = settings.msvc.visual_studio_directory;
        local windows_sdk_directory = settings.msvc.windows_sdk_directory;
        
        local path = {
            visual_studio_directory..[[\Common7\IDE]],
            visual_studio_directory..[[\VC\BIN]],
            visual_studio_directory..[[\Common7\Tools]],
            visual_studio_directory..[[\VC\VCPackages]],
            windows_sdk_directory..[[\bin]],
            getenv("PATH")
        };
        putenv( "PATH", table.concat(path, ";") );
        
        local include = {
            visual_studio_directory..[[\VC\ATLMFC\INCLUDE]],
            visual_studio_directory..[[\VC\INCLUDE]],
            windows_sdk_directory..[[\include]],
            getenv("INCLUDE")
        };
        putenv( "INCLUDE", table.concat(include, ";") );
        
        local lib = {
            visual_studio_directory..[[\VC\ATLMFC\LIB]],
            visual_studio_directory..[[\VC\LIB]],
            windows_sdk_directory..[[\lib]],
            getenv("LIB")
        };
        putenv( "LIB", table.concat(lib, ";") );

        local libpath = {
            visual_studio_directory..[[\VC\ATLMFC\LIB]],
            visual_studio_directory..[[\VC\LIB]],
            getenv("LIBPATH")
        };
        putenv( "LIBPATH", table.concat(libpath, ";") );

        cc = msvc.cc;
        build_library = msvc.build_library;
        clean_library = msvc.clean_library;
        build_executable = msvc.build_executable;
        clean_executable = msvc.clean_executable;
        lipo_executable = msvc.lipo_executable;
        obj_directory = msvc.obj_directory;
        cc_name = msvc.cc_name;
        cxx_name = msvc.cxx_name;
        pch_name = msvc.pch_name;
        pdb_name = msvc.pdb_name;
        obj_name = msvc.obj_name;
        lib_name = msvc.lib_name;
        exp_name = msvc.exp_name;
        dll_name = msvc.dll_name;
        exe_name = msvc.exe_name;
        ilk_name = msvc.ilk_name;
    end
end;

function msvc.cc( target, definition )
    local mscc = definition.settings.msvc.visual_studio_directory.."/VC/bin/cl.exe";

    local cppdefines = "";
    cppdefines = cppdefines.." /DBUILD_OS_"..upper(operating_system());
    cppdefines = cppdefines.." /DBUILD_PLATFORM_"..upper(platform);
    cppdefines = cppdefines.." /DBUILD_VARIANT_"..upper(variant);
    cppdefines = cppdefines.." /DBUILD_LIBRARY_SUFFIX=\"\\\"_"..platform.."_"..variant..".lib\\\"\"";
    cppdefines = cppdefines.." /DBUILD_MODULE_"..upper(string.gsub(target.module:id(), "-", "_"))
    cppdefines = cppdefines.." /DBUILD_LIBRARY_TYPE_"..upper(definition.settings.library_type);

    if definition.settings.defines then
        for _, define in ipairs(definition.settings.defines) do
            cppdefines = cppdefines.." /D"..define;
        end
    end
    
    if definition.defines then
        for _, define in ipairs(definition.defines) do
            cppdefines = cppdefines.." /D"..define;
        end
    end

    local cppdirs = "";
    if definition.include_directories then
        for _, directory in ipairs(definition.include_directories) do
            cppdirs = cppdirs.." /I\""..relative(directory).."\"";
        end
    end

    if definition.settings.include_directories then
        for _, directory in ipairs(definition.settings.include_directories) do
            cppdirs = cppdirs.." /I\""..directory.."\"";
        end
    end

    local ccflags = " /nologo /c";

    if definition.settings.compile_as_c then
        ccflags = ccflags.." /TC";
    else
        ccflags = ccflags.." /TP";
    end
    
    if definition.settings.runtime_library == "static" then
        ccflags = ccflags.." /MT";
    elseif definition.settings.runtime_library == "static_debug" then
        ccflags = ccflags.." /MTd";
    elseif definition.settings.runtime_library == "dynamic" then
        ccflags = ccflags.." /MD";
    elseif definition.settings.runtime_library == "dynamic_debug" then
        ccflags = ccflags.." /MDd";
    end
    
    if definition.settings.debug then
        local pdb = obj_directory(definition)..pdb_name( "msvc.pdb" );
        ccflags = ccflags.." /Zi /Fd"..pdb;
    end

    if definition.settings.exceptions then
        ccflags = ccflags.." /EHsc";
    end

    if definition.settings.link_time_code_generation then
        ccflags = ccflags.." /GL";
    end

    if definition.settings.minimal_rebuild then
        ccflags = ccflags.." /Gm";
    end

    if definition.settings.optimization then
        ccflags = ccflags.." /GF /O2 /Ot /Oi /GS-";
    end

    if definition.settings.preprocess then
        ccflags = ccflags.." /P /C";
    end

    if definition.settings.run_time_type_info then
        ccflags = ccflags.." /GR";
    end

    if definition.settings.run_time_checks then
        ccflags = ccflags.." /RTC1";
    end

    if target.precompiled_header ~= nil then            
        if target.precompiled_header:is_outdated() then
            build.system( mscc, "cl"..cppdirs..cppdefines..ccflags.." /Fp"..obj_directory(definition)..pch_name( target.precompiled_header:id() ).." /Yc"..target.precompiled_header.header.." /Fo"..obj_directory(definition).." ".." "..target.precompiled_header.source );
        end        
        ccflags = ccflags.." /Fp"..obj_directory(definition)..pch_name( target.precompiled_header:id() );
        ccflags = ccflags.." /Yu"..target.precompiled_header.header;
    end
    
    ccsource = "";
    for dependency in target:get_dependencies() do
        if dependency:is_outdated() and dependency ~= target.precompiled_header then            
            if dependency:prototype() == FilePrototype then
                ccsource = ccsource.." "..dependency.source;
            elseif dependency.results then
                for _, result in ipairs(dependency.results) do
                    if result:is_outdated() then
                        ccsource = ccsource.." "..result.source;
                    end
                end
            end
        end    
    end

    if ccsource ~= "" then
        cppdefines = cppdefines.." /DBUILD_VERSION=\"\\\""..version.."\\\"\"";
        build.system( mscc, "cl"..cppdirs..cppdefines..ccflags.." /Fo"..obj_directory(definition).." "..ccsource );
    end
end;

function msvc.build_library( target, definition )
    local msar = definition.settings.msvc.visual_studio_directory.."/VC/bin/lib.exe";

    local arflags = " /nologo";
    
    if definition.settings.link_time_code_generation then
        arflags = arflags.." /ltcg";
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
        build.system( msar, "lib"..arflags.." /out:"..native(target:get_filename())..objects );
        popd();
    end
end;

function msvc.clean_library( target, definition )
    rm( target:get_filename() );
    rmdir( obj_directory(target.module) );    
end;

function msvc.build_executable( target, definition )
    local msld = definition.settings.msvc.visual_studio_directory.."/VC/bin/link.exe";
    local msmt = definition.settings.msvc.windows_sdk_directory.."/bin/mt.exe";
    local msrc = definition.settings.msvc.windows_sdk_directory.."/bin/rc.exe";

    local ldlibs = " advapi32.lib gdi32.lib kernel32.lib user32.lib msimg32.lib ws2_32.lib version.lib odbc32.lib odbccp32.lib";

    local lddirs = " /libpath:\""..definition.settings.lib.."\"";

    if definition.settings.library_directories then
        for _, directory in ipairs(definition.settings.library_directories) do
            lddirs = lddirs.." /libpath:\""..directory.."\"";
        end
    end
    
    local ldflags = " /nologo";

    local intermediate_manifest = obj_directory(target.module)..target:id().."_intermediate.manifest";
    ldflags = ldflags.." /manifest /manifestfile:"..intermediate_manifest;
    
    if definition.settings.subsystem then
        ldflags = ldflags.." /subsystem:"..definition.settings.subsystem;
    end

    ldflags = ldflags.." /out:"..native( target:get_filename() );
    if target.module:prototype() == DynamicLibraryPrototype then
        ldflags = ldflags.." /dll /implib:"..native( "%s/%s.lib" % {definition.settings.lib, basename(target:id())} );
    end
    
    if definition.settings.verbose_linking then
        ldflags = ldflags.." /verbose";
    end
    
    if definition.settings.debug then
        ldflags = ldflags.." /debug /pdb:"..obj_directory(target.module)..pdb_name( target:id() );
    end

    if definition.settings.link_time_code_generation then
        ldflags = ldflags.." /ltcg";
    end

    if definition.settings.generate_map_file then
        ldflags = ldflags.." /map:"..native(obj_directory(target.module)..target:id()..".map");
    end

    if definition.settings.optimization then
        ldflags = ldflags.." /opt:ref /opt:icf";
    end

    if definition.settings.stack_size then
        ldflags = ldflags.." /stack:"..tostring(definition.settings.stack_size);
    end

    local libraries = "";
    if target.libraries then
        for _, library in ipairs(target.libraries) do
            libraries = "%s %s.lib" % { libraries, basename(library:id()) };
        end
    end
    if definition.third_party_libraries then
        for _, library in ipairs(definition.third_party_libraries) do
            libraries = "%s %s" % { libraries, library };
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
        if definition.settings.incremental_linking then
            local embedded_manifest = target:id().."_embedded.manifest";
            local embedded_manifest_rc = target:id().."_embedded_manifest.rc";
            local embedded_manifest_res = target:id().."_embedded_manifest.res";

            if exists(embedded_manifest_rc) ~= true then        
                local rc = io.open( absolute(embedded_manifest_rc), "wb" );
                assert( rc, string.format("Opening '%s' to write manifest failed", absolute(embedded_manifest_rc)) );
                if target:prototype() == ExecutablePrototype then
                    rc:write( "1 /* CREATEPROCESS_MANIFEST_RESOURCE_ID */ 24 /* RT_MANIFEST */ \""..target:id().."_embedded.manifest\"" );
                else
                    rc:write( "2 /* CREATEPROCESS_MANIFEST_RESOURCE_ID */ 24 /* RT_MANIFEST */ \""..target:id().."_embedded.manifest\"" );
                end
                rc:close();
            end

            IgnoreOutputScanner = Scanner {
                [ [[.*]] ] = function()
                end;
            };

            if exists(embedded_manifest) ~= true then        
                build.system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
                build.system( msmt, "mt /nologo /out:\""..embedded_manifest.."\" /manifest "..intermediate_manifest );
                build.system( msrc, "rc /Fo\""..embedded_manifest_res.."\" "..embedded_manifest_rc, IgnoreOutputScanner );
            end

            objects = objects.." "..embedded_manifest_res;
            ldflags = ldflags.." /incremental";
            
            build.system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
            build.system( msmt, "mt /nologo /out:\""..embedded_manifest.."\" /manifest "..intermediate_manifest );
            build.system( msrc, "rc /Fo\""..embedded_manifest_res.."\" "..embedded_manifest_rc, IgnoreOutputScanner );
            build.system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
        else
            ldflags = ldflags.." /incremental:no";
            build.system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
            sleep( 100 );
            build.system( msmt, "mt /nologo -outputresource:"..native(target:get_filename())..";#1 -manifest "..intermediate_manifest );
        end
        popd();
    end
end;

function msvc.clean_executable( target, definition )
    if target.module:prototype() == ExecutablePrototype then
        rm( definition.settings.bin.."/"..exe_name(target:id()) );
        rm( definition.settings.bin.."/"..ilk_name(target:id()) );
        rmdir( obj_directory(target.module) );
    else        
        rm( definition.settings.bin.."/"..dll_name(target:id()) );
        rm( definition.settings.bin.."/"..ilk_name(target:id()) );
        rm( definition.settings.lib.."/"..lib_name(target:id()) );
        rm( definition.settings.lib.."/"..exp_name(target:id()) );
        rmdir( obj_directory(target.module) );
    end        
end;

function msvc.lipo_executable( target )
end

function msvc.obj_directory( target )
    return "%s/%s_%s/%s/" % { target.settings.obj, platform, variant, relative(target:directory(), root()) };
end

function msvc.cc_name( name )
    return "%s.c" % basename( name );
end

function msvc.cxx_name( name )
    return "%s.cpp" % basename( name );
end

function msvc.pch_name( name )
    return "%s.pch" % basename( name );
end

function msvc.pdb_name( name )
    return "%s.pdb" % basename( name );
end

function msvc.obj_name( name )
    return "%s.obj" % basename( name );
end

function msvc.lib_name( name )
    return "%s_%s_%s.lib" % { name, platform, variant };
end

function msvc.exp_name( name )
    return "%s_%s_%s.exp" % { name, platform, variant };
end

function msvc.dll_name( name )
    return "%s_%s_%s.dll" % { name, platform, variant };
end

function msvc.exe_name( name )
    return "%s_%s_%s.exe" % { name, platform, variant };
end

function msvc.ilk_name( name )
    return "%s_%s_%s.ilk" % { name, platform, variant };
end
