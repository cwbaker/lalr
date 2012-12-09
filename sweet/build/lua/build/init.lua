
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

require "build/File";
require "build/SourceFile";
require "build/HeaderFile";
require "build/PrecompiledHeader";
require "build/Directory";
require "build/Compile";
require "build/Archive";
require "build/Link";
require "build/Lipo";
require "build/Source";
require "build/StaticLibrary";
require "build/DynamicLibrary";
require "build/Executable";
require "build/CcScanner";
require "build/commands";

build = {};

-- Perform per run initialization of the build system.
function build.initialize( project_settings )
    platform = platform or build.switch { operating_system(); windows = "msvc"; macosx = "llvmgcc" };
    variant = variant or "debug";
    version = version or "%s %s %s" % { os.date("%Y.%m.%d %H:%M:%S"), platform, variant };
    goal = goal or "";
    jobs = jobs or 4;

    set_maximum_parallel_jobs( jobs );

    local default_settings = build.default_settings;
    build.merge_settings( default_settings, project_settings );

    local local_settings = {};
    setmetatable( local_settings, {__index = default_settings} );

    local user_settings_filename = default_settings.user_settings_filename;
    if exists(user_settings_filename) then
        build.merge_settings( local_settings, dofile(user_settings_filename) );
    end

    local local_settings_filename = default_settings.local_settings_filename;
    if exists(local_settings_filename) then
        build.merge_settings( local_settings, dofile(local_settings_filename) );
    end

    local settings = {};
    setmetatable( settings, {__index = local_settings} );

    local platform_settings = settings.settings_by_platform[platform];
    assert( platform_settings, "The platform '%s' is not supported" % platform );
    build.merge_settings( settings, platform_settings );

    local variant_settings = settings.settings_by_variant[variant];
    assert( variant_settings, "The variant '%s' is not supported" % variant );
    build.merge_settings( settings, variant_settings );

    if settings.library_type == "static" then
        Library = StaticLibrary;
    elseif settings.library_type == "dynamic" then
        Library = DynamicLibrary;
    else
        error( string.format("The library type '%s' is not 'static' or 'dynamic'", settings.library_type) );
    end

    build.default_settings.cache = root( "%s/%s_%s.cache" % {settings.obj, platform, variant} );
    _G.settings = settings;
    build.default_settings = default_settings;
    build.local_settings = local_settings;
    build.settings = settings;
    return settings;
end

-- Visit a target by calling a member function /pass/ if it has one.
function build.visit( pass, ... )
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
function build.built_for_platform_and_variant( target )
    function contains( value, values )
        for _, v in ipairs(values) do
            if v == value then
                return true;
            end
        end        
        return false;
    end    
    return (target.settings.platforms == nil or contains(platform, target.settings.platforms)) and (target.settings.variants == nil or contains(variant, target.settings.variants));
end

-- Execute command with arguments and optional filter and raise an error if 
-- it doesn't return 0.
function build.system( command, arguments, filter )
    if execute(command, arguments, filter) ~= 0 then       
        error( arguments.." failed", 0 );
    end
end

-- Return a value from a table using the first key as a lookup.
function build.switch( values )
    return values[values[1]];
end

-- Dump the keys, values, and prototype of a table for debugging.
function build.dump( t )
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

-- Serialize values to to a Lua file (typically the local settings table).
function build.serialize( file, value, level )
    local function indent( level )
        for i = 0, level - 1 do
            file:write( "  " );
        end
    end

    if type(value) == "boolean" then
        file:write( tostring(value) );
    elseif type(value) == "number" then
        file:write( value );
    elseif type(value) == "string" then
        file:write( string.format("%q", value) );
    elseif type(value) == "table" then
        file:write( "{\n" );
        for _, v in ipairs(value) do
            build.serialize( file, v, level + 1 );
            file:write( ", " );
        end
        for k, v in pairs(value) do
            if type(k) == "string" then
                indent( level + 1 );
                file:write( "%s = " % k );
                build.serialize( file, v, level + 1 );
                file:write( ";\n" );
            end
        end
        indent( level );
        file:write( "}" );
    end
end

-- Save a settings table to a file.
function build.save_settings( settings, filename )
    local file = io.open( filename, "wb" );
    assert( file, "Opening %s to write settings failed" % filename );
    file:write( "\nreturn " );
    build.serialize( file, settings, 0 );
    file:write( "\n" );
    file:close();
end

-- Merge settings from /source_settings/ into /settings/.
function build.merge_settings( settings, source_settings )
    settings = settings or {};
    for _, v in ipairs(source_settings) do
        table.insert( settings, v );
    end
    for k, v in pairs(source_settings) do
        if type(k) == "string" then
            if type(v) == "table" then
                settings[k] = build.merge_settings( settings[k], v );
            else
                settings[k] = v;
            end
        end
    end
    return settings;
end

-- Inherit settings from /settings/ to /target/.
function build.inherit_settings( target, settings )
    local inherited = false;
    if target.settings then
        if not getmetatable(target.settings) then
            setmetatable( target.settings, {__index = settings} );
            inherited = true;
        end
    else
        target.settings = settings;
        inherited = true;
    end
    return inherited;
end

-- Load a target.
function build.load_target( target )
    local inherited = build.inherit_settings( target, build.settings );
    for _, value in ipairs(target) do
        if type(value) == "table" then
            build.inherit_settings( value, target.settings );
        end
    end
    return inherited;
end

-- Load the dependency graph from the file specified by /settings.cache/.
function build.load()
    assert( initialize and type(initialize) == "function", "The 'initialize' function is not defined" );
    assert( buildfiles and type(buildfiles) == "function", "The 'buildfiles' function is not defined" );

    initialize();

    local cache_target = load_binary( settings.cache, initial(goal) );
    if cache_target == nil or cache_target:is_outdated() or build.local_settings.updated then
        clear();
        buildfiles();

        local root_target = find_target( root() );
        assert( root_target , "No root target found at '"..tostring(root()).."'" );
        preorder( build.visit("load"), root_target );
        preorder( build.visit("static_depend"), root_target );
        bind( root_target );

        cache_target = find_target( settings.cache );
        assert( cache_target, "No cache target found at '%s' after loading buildfiles" % settings.cache );
        cache_target:add_dependency( SourceFile(root("build.lua")) );
        cache_target:add_dependency( SourceFile(root("local_settings.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/init.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/File.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/SourceFile.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/HeaderFile.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/PrecompiledHeader.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/Directory.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/Compile.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/Archive.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/Link.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/Lipo.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/Source.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/StaticLibrary.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/DynamicLibrary.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/Executable.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/CcScanner.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/ObjCScanner.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/default_settings.lua")) );
        cache_target:add_dependency( SourceFile(root("build/lua/build/commands.lua")) );
    end

    local all = find_target( initial(goal) );
    assert( all, "No target found at '"..tostring(initial(goal)).."'" );
    bind( all );
    preorder( build.visit("depend"), all );
    bind( all );
end

-- Save the dependency graph to the file specified by /settings.cache/.
function build.save()
    if build.local_settings.updated then
        build.local_settings.updated = nil;
        build.save_settings( build.local_settings, build.settings.local_settings_filename );
    end
    mkdir( branch(settings.cache) );
    save_binary( settings.cache );
end

-- Set default settings (all other settings inherit from this table).
build.default_settings = dofile( root("build/lua/build/default_settings.lua") );
