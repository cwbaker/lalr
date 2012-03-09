
require "build/Cc";
require "build/Parser";
require "build/File";
require "build/SourceFile";
require "build/HeaderFile";
require "build/PrecompiledHeader";
require "build/Directory";
require "build/StaticLibrary";
require "build/DynamicLibrary";
require "build/Executable";
require "build/CcScanner";
require "build/QtMoc";
require "build/AsciiDoc";
require "build/Project";
require "build/msvc";
require "build/visual_studio";

-- Provide python like syntax for string interpolation.
getmetatable("").__mod = function( format, args )
    if args then
        if type(args) == "table" then
            return string.format( format, unpack(args) );
        else
            return string.format( format, args );
        end
    else
        return format;
    end
end

-- Setup the build system.
function setup( settings )
    command  = command or "build";
    platform = platform or "msvc";
    source = source or "";
    target = target or "";
    variant = variant or "debug";
    version = version or os.date( "%Y.%m.%d %H:%M:%S "..platform.." "..variant );
    jobs = jobs or 4;

    set_maximum_parallel_jobs( jobs );

    local default_settings = {
        bin = root();
        lib = root();
        obj = root();

        msvc = {
            visual_studio_directory = autodetect_visual_studio_directory() or "C:/Program Files/Microsoft Visual Studio 9.0";
            windows_sdk_directory = autodetect_windows_sdk_directory() or "C:/Program Files/Microsoft SDKs/Windows/v6.0A";
        };

        parser = {
            executable = "d:/usr/local/bin/parser.exe";
            lua_path = "d:/usr/local/lua/?.lua";
        };

        python = {
            executable = "c:/Python26/python.exe";
        };

        asciidoc = {
            executable = "c:/asciidoc/asciidoc.py";
            conf_file = root( "build/lua/build/sweet.conf" );
        };

        include_directories = {
        };
        
        library_directories = { 
        };

        platforms = {
            "msvc"
        };

        variants = {
            ["debug"] = {
                compile_as_c = false;
                debug = true;
                exceptions = true;
                generate_map_file = true;
                incremental_linking = true;
                library_type = "static";
                link_time_code_generation = false;
                minimal_rebuild = true;
                optimization = false;
                pre_compiled_headers = true;
                preprocess = false;
                profiling = false;
                run_time_checks = true;
                runtime_library = "static_debug";
                run_time_type_info = true;
                stack_size = 1048576;        
                string_pooling = false;
                subsystem = "CONSOLE";
                verbose_linking = false;
            };

            ["debug_dll"] = {
                compile_as_c = false;
                debug = true;
                exceptions = true;
                generate_map_file = true;
                incremental_linking = true;
                library_type = "dynamic";
                link_time_code_generation = false;
                minimal_rebuild = true;
                optimization = false;
                pre_compiled_headers = true;
                preprocess = false;
                profiling = false;
                run_time_checks = true;
                runtime_library = "dynamic_debug";
                run_time_type_info = true;
                stack_size = 1048576;        
                string_pooling = false;
                subsystem = "CONSOLE";
                verbose_linking = false;
            };

            ["release"] = {
                compile_as_c = false;
                debug = true;
                exceptions = true;
                generate_map_file = true;
                incremental_linking = false;
                library_type = "static";
                link_time_code_generation = true;
                minimal_rebuild = false;
                optimization = true;
                pre_compiled_headers = true;
                preprocess = false;
                profiling = false;
                run_time_checks = false;
                runtime_library = "static";
                run_time_type_info = true;
                stack_size = 1048576;        
                string_pooling = false;
                subsystem = "CONSOLE";
                verbose_linking = false;
            };

            ["release_dll"] = {
                compile_as_c = false;
                debug = true;
                exceptions = true;
                generate_map_file = true;
                incremental_linking = false;
                library_type = "dynamic";
                link_time_code_generation = true;
                minimal_rebuild = false;
                optimization = true;
                pre_compiled_headers = true;
                preprocess = false;
                profiling = false;
                run_time_checks = false;
                runtime_library = "dynamic";
                run_time_type_info = true;
                stack_size = 1048576;        
                string_pooling = false;
                subsystem = "CONSOLE";
                verbose_linking = false;
            };
            
            ["shipping"] = {
                compile_as_c = false;
                debug = true;
                exceptions = true;
                generate_map_file = true;
                incremental_linking = false;
                library_type = "static";
                link_time_code_generation = true;
                minimal_rebuild = false;
                optimization = true;
                pre_compiled_headers = true;
                preprocess = false;
                profiling = true;
                run_time_checks = false;
                runtime_library = "static";
                run_time_type_info = true;
                stack_size = 1048576;        
                string_pooling = false;
                subsystem = "CONSOLE";
                verbose_linking = false;
            };
            
            ["shipping_dll"] = {
                compile_as_c = false;
                debug = true;
                exceptions = true;
                generate_map_file = true;
                incremental_linking = false;
                library_type = "dynamic";
                link_time_code_generation = true;
                minimal_rebuild = false;
                optimization = true;
                pre_compiled_headers = true;
                preprocess = false;
                profiling = true;
                run_time_checks = false;
                runtime_library = "dynamic";
                run_time_type_info = true;
                stack_size = 1048576;        
                string_pooling = false;
                subsystem = "CONSOLE";
                verbose_linking = false;
            };    
        };
    };

    if settings then
        setmetatable( settings, {__index = default_settings} );
    else
        settings = default_settings;
    end

    local variant_settings = settings.variants[variant];
    assert( variant_settings, "The variant '"..tostring(variant).."' is not supported" );
    for key, value in pairs(variant_settings) do
        settings[key] = value;
    end

    if settings.library_type == "static" then
        Library = StaticLibrary;
    elseif settings.library_type == "dynamic" then
        Library = DynamicLibrary;
    else
        error( string.format("The library type '%s' is not 'static' or 'dynamic'", settings.library_type) );
    end

    default_settings.root = root();
    default_settings.cache = root( "%s/%s_%s.cache" % {settings.obj, platform, variant} );

    load_binary( settings.cache, initial(target) );
    _G.settings = settings;
end

function build()
    local total_start = 0;

    parser( settings );
    msvc( settings );
    
    local load_start = ticks();
    load_project( project );
    local load_finish = ticks();

    local bind_start = ticks();
    local all = find_target( initial(target) );
    assert( all, "No target found at '"..tostring(initial(target)).."'" );
    preorder( visit("depend"), all );
    bind( all );
    local bind_finish = ticks();

    local build_start = ticks();
    if command == "build" then
        postorder( visit("build"), all );
    elseif command == "generate" then
        postorder( visit("generate"), all );
    elseif command == "document" then
        postorder( visit("document"), all );
    elseif command == "clean" then
        postorder( visit("clean"), all );
        rm( settings.cache );
    elseif command == "compile" then
        compile( source );
    elseif command == "projects" then
        postorder( visit("projects"), all );
    elseif command == "dependencies" then
        print_dependencies( all );
    elseif command == "namespace" then
        print_namespace( all );
    end    
    local build_finish = ticks();

    local save_start = ticks();
    if command == "build" then
        mkdir( branch(settings.cache) );
        save_binary( settings.cache );
    end    
    local save_finish = ticks();

    local total_finish = ticks();
    local load_time = load_finish - load_start;
    local bind_time = bind_finish - bind_start;
    local build_time = build_finish - build_start;
    local save_time = save_finish - save_start;
    local total_time = total_finish - total_start;
    local unknown_time = total_time - (load_time + bind_time + build_time + save_time);
    print( "build: load="..tostring(load_time).."ms, bind="..tostring(bind_time).."ms, build="..tostring(build_time).."ms, save="..tostring(save_time).."ms, unknown="..tostring(unknown_time).."ms, total="..tostring(total_time).."ms" );
end

-- Visit a target by calling a member function /pass/ if it has one.
function visit( pass, ... )
    local args = {...};
    return function( target )
        local fn = target[pass];
        if fn then
            fn( target, unpack(args) );
        end
    end
end

-- Determine whether or not a Target should be built based on whether or not
-- it has platforms and variants attributes that contain the current platform
-- and/or variant.
--
-- @return
--  True if \e target should be built otherwise false.
function built_for_platform_and_variant( target )
    function contains( value, values )
        for _, v in ipairs(values) do
            if v == value then
                return true;
            end
        end        
        return false;
    end    
    return (target.settings.platforms == nil or contains(platform, target.settings.platforms)) and (target.settings.variants == nil or target.settings.variants[variant]);
end

-- Compile the single source file /source/ in /graph/.
function compile( source )
    local source_file = find_target( initial(source) );
    assert( source_file, "No compilable source file found at '"..initial(source).."'" );
    local object_file = source_file.object;
    assert( object_file, "No object file found at '"..obj_directory(source_file.unit)..obj_name(source_file:id()).."'" );
    local unit = source_file.unit;
    for dependency in unit:get_dependencies() do
        if dependency:rule() == File and dependency ~= unit.precompiled_header then
            dependency:set_outdated( false );
        end
    end
    unit:set_outdated( true );
    object_file:set_outdated( true );
    postorder( visit("build"), unit );
end

-- Execute command with arguments and optional filter and raise an error if 
-- it doesn't return 0.
function system( command, arguments, filter )
    if execute(command, arguments, filter) ~= 0 then       
        error( arguments.." failed" );
    end
end

-- Dump the keys, values, and prototype of a table for debugging.
function dump( t )
    print( tostring(t) );
    if t ~= nil then
        if getmetatable(t) ~= nil then
            print( "  prototype="..tostring(getmetatable(t).__index) );
        end
        for k, v in pairs(t) do
            print( "  "..tostring(k).." -> "..tostring(v) );
        end
    end
end

-- Load a project.
function load_project( project )
    local root_target = find_target( root() );
    assert( root_target , "No root target found at '"..tostring(root()).."'" );
    if not root_target.loaded then
        root_target.loaded = true;
        preorder( visit("load"), root_target );
        preorder( visit("static_depend"), root_target );

        local cache = find_target( settings.cache );
        cache.loaded = true;
        cache:add_dependency( SourceFile("build.lua") );
        cache:add_dependency( SourceFile("build/lua/build/Cc.lua") );
        cache:add_dependency( SourceFile("build/lua/build/CcScanner.lua") );
        cache:add_dependency( SourceFile("build/lua/build/settings.lua") );
        cache:add_dependency( SourceFile("build/lua/build/Directory.lua") );
        cache:add_dependency( SourceFile("build/lua/build/Executable.lua") );
        cache:add_dependency( SourceFile("build/lua/build/File.lua") );
        cache:add_dependency( SourceFile("build/lua/build/HeaderFile.lua") );
        cache:add_dependency( SourceFile("build/lua/build/init.lua") );
        cache:add_dependency( SourceFile("build/lua/build/StaticLibrary.lua") );
        cache:add_dependency( SourceFile("build/lua/build/DynamicLibrary.lua") );
        cache:add_dependency( SourceFile("build/lua/build/Parser.lua") );
        cache:add_dependency( SourceFile("build/lua/build/Project.lua") );
        cache:add_dependency( SourceFile("build/lua/build/SourceFile.lua") );
        cache:add_dependency( SourceFile("build/lua/build/QtMoc.lua") );
        cache:add_dependency( SourceFile("build/lua/build/AsciiDoc.lua") );
        cache:add_dependency( SourceFile("build/lua/build/msvc.lua") );
        cache:add_dependency( SourceFile("build/lua/build/visual_studio.lua") );
        local user_settings = home( "user_settings.lua" );
        if exists(user_settings) then
            cache:add_dependency( SourceFile(user_settings) );
        end
    end
end

-- Load a module.
function load_module( module )
    if module.settings then
        setmetatable( module.settings, {__index = settings} );
    else
        module.settings = settings;
    end
    
    local settings = module.settings;
    for _, unit in ipairs(module) do        
        if unit.settings then
            setmetatable( unit.settings, {__index = settings} );        
        else
            unit.settings = settings;
        end
        module:add_dependency( unit );
        unit.module = module;
    end

    local project = Project( absolute(string.format("%s.project", module.project_name or module:id())) );
    project.module = module;
    
    local working_directory = module:get_working_directory();
    working_directory:add_dependency( project );
end
