
msvc = {};

function msvc.configure( settings )
    local function registry( key )
        local values = {};
        local reg = "C:/Windows/system32/reg.exe";
        local arguments = ('reg query "%s"'):format( key );
        build:system( reg, arguments, nil, nil, function(line)
            local REG_QUERY_PATTERN = "%s* ([%w_]+) %s* ([%w_]+) %s* ([^\n\r]+)";
            local key, type_, value = line:match( REG_QUERY_PATTERN );
            if key and type_ and value then 
                values[key] = value;
            end
        end );
        return values;
    end

    local function autodetect_visual_studio_directory()
        local visual_studio_directory = os.getenv( "VS120COMNTOOLS" );
        if visual_studio_directory then
            visual_studio_directory = string.gsub( visual_studio_directory, "\\Common7\\Tools\\", "" );
        end    
        return visual_studio_directory;
    end

    local function autodetect_windows_sdk_directory()
        local windows_sdk = registry( [[HKLM\SOFTWARE\Microsoft\Windows Kits\Installed Roots]] );
        if windows_sdk then 
            return windows_sdk.KitsRoot81 or windows_sdk.KitsRoot;
        end
    end

    if build:operating_system() == "windows" then
        local local_settings = build.local_settings;
        if not local_settings.msvc then
            local_settings.updated = true;
            local_settings.msvc = {
                visual_studio_directory = autodetect_visual_studio_directory() or "C:/Program Files (x86)/Microsoft Visual Studio 12.0";
                windows_sdk_directory = autodetect_windows_sdk_directory();
            };
        end
    end
end;

function msvc.initialize( settings )
    if build:platform_matches("windows") then
        -- Make sure that the environment variable VS_UNICODE_OUTPUT is not set.
        --
        -- Visual Studio sets this to signal its tools to communicate back to 
        -- Visual Studio using named pipes rather than stdout so that unicode output 
        -- works better but this then prevents the build tool from intercepting
        -- and collating this output.
        --
        -- See http://blogs.msdn.com/freik/archive/2006/04/05/569025.aspx.
        -- putenv( "VS_UNICODE_OUTPUT", "" );

        local visual_studio_directory = settings.msvc.visual_studio_directory;
        
        local path_i386 = {
            ('%s\\VC\\bin\\amd64_x86'):format( visual_studio_directory ),
            ('%s\\Common7\\IDE'):format( visual_studio_directory ),
            ('%s\\Common7\\Tools'):format( visual_studio_directory ),
            ('%s\\VC\\vcpackages'):format( visual_studio_directory ),
        };

        local path_x86_64 = {
            ('%s\\VC\\bin\\amd64'):format( visual_studio_directory ),
            ('%s\\Common7\\IDE'):format( visual_studio_directory ),
            ('%s\\Common7\\Tools'):format( visual_studio_directory ),
            ('%s\\VC\\vcpackages'):format( visual_studio_directory ),
        };
        
        local lib_i386 = {
            ('%s\\VC\\atlmfc\\lib'):format( visual_studio_directory ),
            ('%s\\VC\\lib'):format( visual_studio_directory )
        };

        local lib_x86_64 = {
            ('%s\\VC\\atlmfc\\lib\\amd64'):format( visual_studio_directory ),
            ('%s\\VC\\lib\\amd64'):format( visual_studio_directory )
        };

        local include = {
            ('%s\\VC\\atlmfc\\include'):format( visual_studio_directory ),
            ('%s\\VC\\include'):format( visual_studio_directory ),
        };
        
        local windows_sdk_directory = settings.msvc.windows_sdk_directory;
        if windows_sdk_directory then 
            table.insert( include, ('%s\\Include\\um'):format(windows_sdk_directory) );
            table.insert( include, ('%s\\Include\\shared'):format(windows_sdk_directory) );
            table.insert( include, ('%s\\Include\\winrt'):format(windows_sdk_directory) );
            table.insert( lib_i386, ('%s\\Lib\\winv6.3\\um\\x86'):format(windows_sdk_directory) );
            table.insert( lib_x86_64, ('%s\\Lib\\winv6.3\\um\\x64'):format(windows_sdk_directory) );
        end

        msvc.environments_by_architecture = {
            ["i386"] = {
                PATH = table.concat( path_i386, ";" );
                LIB = table.concat( lib_i386, ";" );
                LIBPATH = table.concat( lib_i386, ";" );
                INCLUDE = table.concat( include, ";" );
                SYSTEMROOT = os.getenv( "SYSTEMROOT" );
                TMP = os.getenv( "TMP" );
            };
            ["x86_64"] = {
                PATH = table.concat( path_x86_64, ";" );
                LIB = table.concat( lib_x86_64, ";" );
                LIBPATH = table.concat( lib_x86_64, ";" );
                INCLUDE = table.concat( include, ";" );
                SYSTEMROOT = os.getenv( "SYSTEMROOT" );
                TMP = os.getenv( "TMP" );
            };
        };
    end
end;

function msvc.visual_studio_tool( target, tool )
    if target.architecture == "x86_64" then
        return ("%s/VC/bin/amd64/%s"):format( target.settings.msvc.visual_studio_directory, tool );
    else
        return ("%s/VC/bin/%s"):format( target.settings.msvc.visual_studio_directory, tool );
    end
end

function msvc.windows_sdk_tool( target, tool )
    if target.architecture == "x86_64" then
        return ("%s/bin/x64/%s"):format( target.settings.msvc.windows_sdk_directory, tool );
    else
        return ("%s/bin/x86/%s"):format( target.settings.msvc.windows_sdk_directory, tool );
    end
end

function msvc.append_defines( target, flags )
    table.insert( flags, ('/DBUILD_PLATFORM_%s'):format(build:upper(platform)) );
    table.insert( flags, ('/DBUILD_VARIANT_%s'):format(build:upper(variant)) );
    table.insert( flags, ('/DBUILD_LIBRARY_SUFFIX="\\"_%s.lib\\""'):format(target.architecture) );
    table.insert( flags, ('/DBUILD_LIBRARY_TYPE_%s'):format(build:upper(target.settings.library_type)) );
    table.insert( flags, '/D_CRT_SECURE_NO_WARNINGS' );

    if string.find(target.settings.runtime_library, "debug", 1, true) then
        table.insert( flags, "/D_DEBUG" );
        table.insert( flags, "/DDEBUG" );
    else 
        table.insert( flags, "/DNDEBUG" );
    end

    if target.settings.defines then
        for _, define in ipairs(target.settings.defines) do
            table.insert( flags, ("/D%s"):format(define) );
        end
    end

    if target.defines then
        for _, define in ipairs(target.defines) do
            table.insert( flags, ("/D%s"):format(define) );
        end
    end
end

function msvc.append_version_defines( target, flags )
    table.insert( flags, ('/DBUILD_VERSION="\\"%s\\""'):format(version) );
end

function msvc.append_include_directories( target, flags )
    if target.include_directories then
        for _, directory in ipairs(target.include_directories) do
            table.insert( flags, ('/I "%s"'):format(build:relative(directory)) );
        end
    end

    if target.settings.include_directories then
        for _, directory in ipairs(target.settings.include_directories) do
            table.insert( flags, ('/I "%s"'):format(directory) );
        end
    end
end

function msvc.append_compile_flags( target, flags )
    table.insert( flags, "/nologo" );
    table.insert( flags, "/FS" );
    table.insert( flags, "/FC" );
    table.insert( flags, "/c" );
    table.insert( flags, "/showIncludes" );

    local language = target.language or "c++";
    if language == "c" then 
        table.insert( flags, "/TC" );
    elseif language == "c++" then
        table.insert( flags, "/TP" );
        if target.settings.exceptions then
            table.insert( flags, "/EHsc" );
        end
        if target.settings.run_time_type_info then
            table.insert( flags, "/GR" );
        end
    else
        assert( false, "Only the 'c' and 'c++' languages are supported by Microsoft Visual C++" );
    end

    if target.settings.runtime_library == "static" then
        table.insert( flags, "/MT" );
    elseif target.settings.runtime_library == "static_debug" then
        table.insert( flags, "/MTd" );
    elseif target.settings.runtime_library == "dynamic" then
        table.insert( flags, "/MD" );
    elseif target.settings.runtime_library == "dynamic_debug" then
        table.insert( flags, "/MDd" );
    end
    
    if target.settings.debug then
        local pdb = ("%s%s"):format(obj_directory(target), pdb_name(target:working_directory():id()) );
        table.insert( flags, ("/Zi /Fd%s"):format(build:native(pdb)) );
    end

    if target.settings.link_time_code_generation then
        table.insert( flags, "/GL" );
    end

    if target.settings.minimal_rebuild then
        table.insert( flags, "/Gm" );
    end

    if target.settings.optimization then
        table.insert( flags, "/GF /O2 /Ot /Oi /Ox /Oy /GS- /favor:blend" );
    end

    if target.settings.preprocess then
        table.insert( flags, "/P /C" );
    end

    if target.settings.run_time_checks then
        table.insert( flags, "/RTC1" );
    end

    if target.settings.warnings_as_errors then 
        table.insert( flags, "/WX" );
    end

    local warning_level = target.settings.warning_level
    if warning_level == 0 then 
        table.insert( flags, "/w" );
    elseif warning_level == 1 then
        table.insert( flags, "/W1" );
    elseif warning_level == 2 then
        table.insert( flags, "/W2" );
    elseif warning_level == 3 then
        table.insert( flags, "/W3" );
    elseif warning_level >= 4 then
        table.insert( flags, "/W4" );
    end
end

function msvc.append_library_directories( target, flags )
    if target.library_directories then
        for _, directory in ipairs(target.library_directories) do
            table.insert( flags, ('/libpath:"%s"'):format(directory) );
        end
    end    

    if target.settings.library_directories then
        for _, directory in ipairs(target.settings.library_directories) do
            table.insert( flags, ('/libpath:"%s"'):format(directory) );
        end
    end    
end

function msvc.append_link_flags( target, flags )
    table.insert( flags, "/nologo" );

    local intermediate_manifest = ('%s%s_intermediate.manifest'):format( obj_directory(target), target:id() );
    table.insert( flags, "/manifest" );
    table.insert( flags, ("/manifestfile:%s"):format(intermediate_manifest) );
    
    if target.settings.subsystem then
        table.insert( flags, ("/subsystem:%s"):format(target.settings.subsystem) );
    end

    table.insert( flags, ("/out:%s"):format(build:native(target:filename())) );
    if target:prototype() == build.DynamicLibrary then
        table.insert( flags, "/dll" );
        table.insert( flags, ("/implib:%s"):format(build:native(("%s/%s.lib"):format(target.settings.lib, target:id()))) );
    end
    
    if target.settings.verbose_linking then
        table.insert( flags, "/verbose" );
    end
    
    if target.settings.debug then
        table.insert( flags, "/debug" );
        table.insert( flags, ("/pdb:%s"):format(build:native(obj_directory(target), pdb_name(target:id()))) );
    end

    if target.settings.link_time_code_generation then
        table.insert( flags, "/ltcg" );
    end

    if target.settings.generate_map_file then
        table.insert( flags, ("/map:%s.map"):format(build:native(("%s%s"):format(obj_directory(target), target:id()))) );
    end

    if target.settings.optimization then
        table.insert( flags, "/opt:ref" );
        table.insert( flags, "/opt:icf" );
    end

    if target.settings.stack_size then
        table.insert( flags, ("/stack:%d"):format(target.settings.stack_size) );
    end
end

function msvc.append_link_libraries( target, flags )
    if target.settings.third_party_libraries then
        for _, library in ipairs(target.settings.third_party_libraries) do
            table.insert( flags, ("%s.lib"):format(build:basename(library)) );
        end
    end
    if target.third_party_libraries then
        for _, library in ipairs(target.settings.third_party_libraries) do
            table.insert( flags, ("%s.lib"):format(build:basename(library)) );
        end
    end
    if target.system_libraries then
        for _, library in ipairs(target.system_libraries) do
            table.insert( flags, ("%s.lib"):format(build:basename(library)) );
        end
    end
end

function msvc.dependencies_filter( output_directory, source_directory )
    local object;
    local current_directory = source_directory;
    local directories = { source_directory };

    -- Strip the backslash delimited prefix from _include_path_ and return the
    -- remaining portion.  This remaining portion is the correct relative path to
    -- a header.
    local function relative_include_path( include_path )
        local position = 1;
        local start, finish = include_path:find( "\\", position, false );
        while start do 
            position = finish + 1;
            start, finish = include_path:find( "\\", position, false );
        end
        return include_path:sub( position );
    end

    -- Match lines that indicate source files and header files in Microsoft
    -- Visual C++ output to gather dependencies for source file compilation.
    local function dependencies_filter( line )
        local SHOW_INCLUDES_PATTERN = "^Note: including file:(%s*)([^\n\r]*)[\n\r]*$";
        local indent, path = line:match( SHOW_INCLUDES_PATTERN );
        if indent and path then
            local indent = #indent;
            if indent < #directories then
                while indent < #directories do 
                    table.remove( directories );
                end
            end
            if indent > #directories then 
                table.insert( directories, current_directory );
            end

            local LOWER_CASE_DRIVE_PATTERN = "^%l:";
            local lower_case_path = path:find( LOWER_CASE_DRIVE_PATTERN );
            if lower_case_path then 
                local directory = directories[#directories];
                path = ("%s/%s"):format( directory, relative_include_path(path) );
            end

            local within_source_tree = build:relative( path, build:root() ):find( "..", 1, true ) == nil;
            if within_source_tree then
                local header = build:SourceFile( path );
                object:add_dependency( header );
            end
            current_directory = build:branch( path );
        else
            local SOURCE_FILE_PATTERN = "^[%w_]+%.[%w_]+[\n\r]*$";
            local start, finish = line:find( SOURCE_FILE_PATTERN );
            if start and finish then 
                object = build:File( ("%s/%s"):format(output_directory, obj_name(line)) );
            end
            printf( line );
        end
    end
    return dependencies_filter;
end

build:register_module( msvc );
