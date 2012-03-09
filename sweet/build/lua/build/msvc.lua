
function autodetect_visual_studio_directory()
    local visual_studio_directory = os.getenv( "VS90COMNTOOLS" ) or os.getenv( "VS100COMNTOOLS" );
    if visual_studio_directory then
        visual_studio_directory = string.gsub( visual_studio_directory, "\\Common7\\Tools\\", "" );
    end    
    return visual_studio_directory;
end

function autodetect_windows_sdk_directory()
    local windows_sdk_directory = os.getenv( "WindowsSdkDir" );
    return windows_sdk_directory;
end

function msvc( settings )
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
end

function cc( target )
    local mscc = target.settings.msvc.visual_studio_directory.."/VC/bin/cl.exe";

    local cppdefines = "";
    cppdefines = cppdefines.." /DBUILD_PLATFORM_"..upper(platform);
    cppdefines = cppdefines.." /DBUILD_VARIANT_"..upper(variant);
    cppdefines = cppdefines.." /DBUILD_LIBRARY_SUFFIX=\"\\\"_"..platform.."_"..variant..".lib\\\"\"";
    cppdefines = cppdefines.." /DBUILD_MODULE_"..upper(string.gsub(target.module:id(), "-", "_"))
    cppdefines = cppdefines.." /DBUILD_LIBRARY_TYPE_"..upper(target.settings.library_type);

    if target.settings.defines then
        for _, define in ipairs(target.settings.defines) do
            cppdefines = cppdefines.." /D"..define;
        end
    end
    
    if target.defines then
        for _, define in ipairs(target.defines) do
            cppdefines = cppdefines.." /D"..define;
        end
    end

    local cppdirs = "";
    if target.include_directories then
        for _, directory in ipairs(target.include_directories) do
            cppdirs = cppdirs.." /I\""..relative(directory).."\"";
        end
    end

    if target.settings.include_directories then
        for _, directory in ipairs(target.settings.include_directories) do
            cppdirs = cppdirs.." /I\""..directory.."\"";
        end
    end

    local ccflags = " /nologo /c";

    if target.settings.compile_as_c then
        ccflags = ccflags.." /TC";
    else
        ccflags = ccflags.." /TP";
    end
    
    if target.settings.runtime_library == "static" then
        ccflags = ccflags.." /MT";
    elseif target.settings.runtime_library == "static_debug" then
        ccflags = ccflags.." /MTd";
    elseif target.settings.runtime_library == "dynamic" then
        ccflags = ccflags.." /MD";
    elseif target.settings.runtime_library == "dynamic_debug" then
        ccflags = ccflags.." /MDd";
    end
    
    if target.settings.debug then
        local pdb = obj_directory(target)..pdb_name( "msvc.pdb" );
        ccflags = ccflags.." /Zi /Fd"..pdb;
    end

    if target.settings.exceptions then
        ccflags = ccflags.." /EHsc";
    end

    if target.settings.link_time_code_generation then
        ccflags = ccflags.." /GL";
    end

    if target.settings.minimal_rebuild then
        ccflags = ccflags.." /Gm";
    end

    if target.settings.optimization then
        ccflags = ccflags.." /GF /O2 /Ot /Oi /GS-";
    end

    if target.settings.preprocess then
        ccflags = ccflags.." /P /C";
    end

    if target.settings.run_time_type_info then
        ccflags = ccflags.." /GR";
    end

    if target.settings.run_time_checks then
        ccflags = ccflags.." /RTC1";
    end

    if target.precompiled_header ~= nil then            
        if target.precompiled_header:is_outdated() then
            system( mscc, "cl"..cppdirs..cppdefines..ccflags.." /Fp"..obj_directory(target)..pch_name( target.precompiled_header:id() ).." /Yc"..target.precompiled_header.header.." /Fo"..obj_directory(target).." ".." "..target.precompiled_header.source );
        end        
        ccflags = ccflags.." /Fp"..obj_directory(target)..pch_name( target.precompiled_header:id() );
        ccflags = ccflags.." /Yu"..target.precompiled_header.header;
    end
    
    ccsource = "";
    for dependency in target:get_dependencies() do
        if dependency:is_outdated() and dependency ~= target.precompiled_header then
            if dependency:rule() == File then
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
        system( mscc, "cl"..cppdirs..cppdefines..ccflags.." /Fo"..obj_directory(target).." "..ccsource );
    end
end

function build_library( target )
    local msar = target.settings.msvc.visual_studio_directory.."/VC/bin/lib.exe";

    local arflags = " /nologo";
    
    if target.settings.link_time_code_generation then
        arflags = arflags.." /ltcg";
    end
    
    local objects = "";
    for dependency in target:get_dependencies() do
        if dependency:rule() == Cc then
            if dependency.precompiled_header ~= nil then
                objects = objects.." "..obj_name( dependency.precompiled_header:id() );
            end
            
            for object in dependency:get_dependencies() do
                if object:rule() == File and object ~= dependency.precompiled_header then
                    objects = objects.." "..obj_name( object:id() );
                end
            end
        end
    end
    
    if objects ~= "" then
        print( lib_name(target:id()) );
        pushd( obj_directory(target) );
        system( msar, "lib"..arflags.." /out:"..native(target:get_filename())..objects );
        popd();
    end
end

function clean_library( target )
    rm( target.settings.lib.."/"..lib_name(target:id()) );
    rmdir( obj_directory(target) );    
end

function build_executable( target )
    local msld = target.settings.msvc.visual_studio_directory.."/VC/bin/link.exe";
    local msmt = target.settings.msvc.windows_sdk_directory.."/bin/mt.exe";
    local msrc = target.settings.msvc.windows_sdk_directory.."/bin/rc.exe";

    local ldlibs = " advapi32.lib gdi32.lib kernel32.lib user32.lib msimg32.lib ws2_32.lib version.lib odbc32.lib odbccp32.lib";

    local lddirs = " /libpath:\""..target.settings.lib.."\"";

    if target.settings.library_directories then
        for _, directory in ipairs(target.settings.library_directories) do
            lddirs = lddirs.." /libpath:\""..directory.."\"";
        end
    end
    
    local ldflags = " /nologo";

    local intermediate_manifest = obj_directory(target)..target:id().."_intermediate.manifest";
    ldflags = ldflags.." /manifest /manifestfile:"..intermediate_manifest;
    
    if target.settings.subsystem then
        ldflags = ldflags.." /subsystem:"..target.settings.subsystem;
    end

    local out = "";
    if target:rule() == Executable then
        out = native( target:get_filename() );
        ldflags = ldflags.." /out:"..out;
    elseif target:rule() == Library then
        out = native( target.settings.bin.."/"..dll_name(target:id()) );
        ldflags = ldflags.." /out:"..out;
        ldflags = ldflags.." /dll /implib:"..native( target.settings.lib.."/"..lib_name(target:id()) );
    end
    
    if target.settings.verbose_linking then
        ldflags = ldflags.." /verbose";
    end
    
    if target.settings.debug then
        ldflags = ldflags.." /debug /pdb:"..obj_directory(target)..pdb_name( target:id() );
    end

    if target.settings.link_time_code_generation then
        ldflags = ldflags.." /ltcg";
    end

    if target.settings.generate_map_file then
        ldflags = ldflags.." /map:"..native(obj_directory(target)..target:id()..".map");
    end

    if target.settings.optimization then
        ldflags = ldflags.." /opt:ref /opt:icf";
    end

    if target.settings.stack_size then
        ldflags = ldflags.." /stack:"..tostring(target.settings.stack_size);
    end

    local libraries = "";
    if target.libraries then
        for _, library in ipairs(target.libraries) do
            libraries = libraries.." "..lib_name( library:id() );
        end
    end

    local objects = "";
    for dependency in target:get_dependencies() do
        if dependency:rule() == Cc then
            if dependency.precompiled_header ~= nil then
                objects = objects.." "..obj_name( dependency.precompiled_header:id() );
            end
            
            for object in dependency:get_dependencies() do
                if object:rule() == File and object ~= dependency.precompiled_header then
                    objects = objects.." "..obj_name( object:id() );
                end
            end
        end
    end

    if objects ~= "" then
        if target:rule() == Executable then
            print( exe_name(target:id()) );
        else
            print( dll_name(target:id()) );
        end

        pushd( obj_directory(target) );
        if target.settings.incremental_linking then
            local embedded_manifest = target:id().."_embedded.manifest";
            local embedded_manifest_rc = target:id().."_embedded_manifest.rc";
            local embedded_manifest_res = target:id().."_embedded_manifest.res";

            if exists(embedded_manifest_rc) ~= true then        
                local rc = io.open( absolute(embedded_manifest_rc), "wb" );
                assert( rc, string.format("Opening '%s' to write manifest failed", absolute(embedded_manifest_rc)) );
                if target:rule() == Executable then
                    rc:write( "1 /* CREATEPROCESS_MANIFEST_RESOURCE_ID */ 24 /* RT_MANIFEST */ \""..target:id().."_embedded.manifest\"" );
                else
                    rc:write( "2 /* CREATEPROCESS_MANIFEST_RESOURCE_ID */ 24 /* RT_MANIFEST */ \""..target:id().."_embedded.manifest\"" );
                end
                rc:close();
            end

            if exists(embedded_manifest) ~= true then        
                system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
                system( msmt, "mt /nologo /out:\""..embedded_manifest.."\" /manifest "..intermediate_manifest );
                system( msrc, "rc /Fo\""..embedded_manifest_res.."\" "..embedded_manifest_rc );
            end

            objects = objects.." "..embedded_manifest_res;
            ldflags = ldflags.." /incremental";
            
            system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
            system( msmt, "mt /nologo /out:\""..embedded_manifest.."\" /manifest "..intermediate_manifest );
            system( msrc, "rc /Fo\""..embedded_manifest_res.."\" "..embedded_manifest_rc );
            system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
        else
            ldflags = ldflags.." /incremental:no";
            system( msld, "link"..ldflags..lddirs..objects..libraries..ldlibs );
            sleep( 100 );
            system( msmt, "mt /nologo -outputresource:"..out..";#1 -manifest "..intermediate_manifest );
        end
        popd();
    end
end 

function clean_executable( target )
    if target:rule() == Executable then
        rm( target.settings.bin.."/"..exe_name(target:id()) );
        rm( target.settings.bin.."/"..ilk_name(target:id()) );
        rmdir( obj_directory(target) );
    else        
        rm( target.settings.bin.."/"..dll_name(target:id()) );
        rm( target.settings.bin.."/"..ilk_name(target:id()) );
        rm( target.settings.lib.."/"..lib_name(target:id()) );
        rm( target.settings.lib.."/"..exp_name(target:id()) );
        rmdir( obj_directory(target) );
    end        
end

function obj_directory( target )
    return target.settings.obj.."/"..platform.."_"..variant.."/"..relative( target:directory(), root() ).."/";
end

function cc_name( name )
    return basename( name )..".c";
end

function cxx_name( name )
    return basename( name )..".cpp";
end

function pch_name( name )
    return basename( name )..".pch";
end

function pdb_name( name )
    return basename( name )..".pdb";
end

function obj_name( name )
    return basename( name )..".obj";
end

function lib_name( name )
    return name.."_"..platform.."_"..variant..".lib";
end

function exp_name( name )
    return name.."_"..platform.."_"..variant..".exp";
end

function dll_name( name )
    return name.."_"..platform.."_"..variant..".dll";
end

function exe_name( name )
    return name.."_"..platform.."_"..variant..".exe";
end

function ilk_name( name )
    return name.."_"..platform.."_"..variant..".ilk";
end
