
-- Provide printf().
function printf( format, ... ) 
    print( string.format(format, ...) );
end

-- Provide formatted assert().
function assertf( condition, format, ... )
    if not condition then 
        assert( condition, string.format(format, ...) );
    end
end

-- Provide formatted error().
function errorf( format, ... )
    error( string.format(format, ...) );
end

build.buildfiles_stack = {};

function build.current_buildfile()
    local buildfiles_stack = build.buildfiles_stack;
    local back = #buildfiles_stack;
    if back > 0 then 
        return buildfiles_stack[back];
    end
end

-- Provide buildfile() that restores the settings stack position.
local original_buildfile = build.buildfile;
function buildfile( ... )
    local position = build.store_settings();
    local buildfiles_stack = build.buildfiles_stack;
    table.insert( buildfiles_stack, build.file(select(1, ...)) );
    local success, errors_or_error_message = pcall( original_buildfile, ... );
    table.remove( buildfiles_stack );
    build.restore_settings( position );
    assertf( success and errors_or_error_message == 0, "Loading buildfile '%s' failed", tostring(select(1, ...)) );
end

build = _G.build or {};

function build.matches( value, ... )
    for i = 1, select("#", ...) do 
        if string.match(value, select(i, ...)) then
            return true;
        end
    end
    return false;
end

function build.platform_matches( ... )
    return platform == "" or build.matches( platform, ... );
end

function build.variant_matches( ... )
    return variant == "" or build.matches( variant, ... );
end

function build.default_create( target_prototype, ... )
    local identifier_or_sources = select( 1, ... );
    if type(identifier_or_sources) ~= "table" then   
        local settings = build.current_settings();
        local identifier = build.interpolate( identifier_or_sources, settings );
        local target = build.Target( identifier, target_prototype, select(2, ...) );
        if build.is_relative(identifier) then 
            target:set_filename( build.object(identifier) );
        else
            target:set_filename( identifier );
        end
        target:set_cleanable( true );
        target:add_ordering_dependency( build.Directory(build.branch(target)) );
        target.settings = settings;
        return target;
    end
end

function build.default_depend( target, dependencies )
    local settings = build.current_settings();
    if type(dependencies) == "string" then
        local source_file = build.SourceFile( dependencies, settings );
        target:add_dependency( source_file );
    elseif type(dependencies) == "table" then
        build.merge( target, dependencies );
        for _, value in ipairs(dependencies) do 
            local source_file = build.SourceFile( value, settings );
            target:add_dependency( source_file );
        end
    end
    return target;
end

function build.TargetPrototype( id, create_function )
    -- Split a '.' delimited string into a table hierarchy returning the last 
    -- level table and identifier.
    local function split_modules( module, qualified_id )
        local start = qualified_id:find( ".", 1, true );
        if start then
            local id = qualified_id:sub( 1, start - 1 );
            local submodule = module[id];
            if submodule == nil then 
                submodule = {};
                module[id] = submodule;
            end
            local remaining = qualified_id:sub( start + 1 );
            return split_modules( submodule, remaining );
        end
        return module, qualified_id;
    end

    local target_prototype = build.target_prototype( id );
    getmetatable( target_prototype ).__call = create_function or function( target_prototype, ... )
        local create_function = target_prototype.create;
        if create_function then 
            local settings = build.current_settings();
            return create_function( settings, ... );
        end
        return build.default_create( target_prototype, ... );
    end;
    local module, id = split_modules( build, id );
    module[id] = target_prototype;
    return target_prototype;
end

function build.Target( id, target_prototype, definition )
    local target_ = build.target( id, target_prototype, definition );
    getmetatable( target_ ).__call = function( target, ... )
        local depend_function = target.call or target.depend or build.default_depend;
        depend_function( target, ... );
        return target;
    end;
    return target_;
end

function build.File( filename, target_prototype, definition )
    local settings = build.current_settings();
    local filename = build.interpolate( filename, settings );
    local target = build.file( filename, target_prototype, definition );
    target:set_cleanable( true );
    getmetatable( target ).__call = function( target, ... )
        local depend_function = target.call or target.depend or build.default_depend;
        depend_function( target, ... );
        return target;
    end;
    return target;
end

function build.GeneratedFile( filename, target_prototype, definition )
    local target_ = build.file( filename, target_prototype, definition );
    target_:set_cleanable( false );
    getmetatable( target_ ).__call = function( target, ... )
        local depend_function = target.call or target.depend or build.default_depend;
        depend_function( target, ... );
        return target;
    end;
    return target_;
end

function build.SourceFile( value, settings )
    local target = value;
    if type(target) == "string" then 
        settings = settings or build.current_settings();
        target = build.file( build.interpolate(value, settings) );
        target:set_required_to_exist( true );
        target:set_cleanable( false );
    end
    return target;
end

function build.ImplicitSourceFile( value, settings )
    local target = value;
    if type(target) == "string" then 
        settings = settings or build.current_settings();
        target = build.file( build.interpolate(value, settings) );
        target:set_cleanable( false );
    end
    return target;
end

function build.SourceDirectory( value, settings )
    return build.SourceFile( value, settings );
end

function build.map( target_prototype, replacement, pattern, filenames )
    local targets = {};
    local settings = settings or build.current_settings();
    for _, source_filename in ipairs(filenames) do 
        local source = build.relative( source_filename );
        local filename, substitutions = source:gsub( pattern, replacement );
        if substitutions > 0 then 
            local destination = build.interpolate( filename, settings );
            local target = target_prototype (destination) (source);
            table.insert( targets, target );
        end
    end
    return table.unpack( targets );
end

function build.map_ls( target_prototype, replacement, pattern, settings )
    local targets = {};
    local settings = settings or build.current_settings();
    for source_filename in build.ls("") do
        local source = build.relative( source_filename );
        local filename, substitutions = source:gsub( pattern, replacement );
        if substitutions > 0 then    
            local destination = build.interpolate( filename, settings );
            local target = target_prototype (destination) (source);
            table.insert( targets, target );
        end
    end
    return table.unpack( targets );
end

function build.map_find( target_prototype, replacement, pattern, settings )
    local targets = {};
    local settings = settings or build.current_settings();
    for source_filename in build.find("") do
        if build.is_file(source_filename) then
            local source = build.relative( source_filename );
            local filename, substitutions = source:gsub( pattern, replacement );
            if substitutions > 0 then
                local destination = build.interpolate( filename, settings );
                local target = target_prototype (destination) (source);
                table.insert( targets, target );
            end
        end
    end
    return table.unpack( targets );
end

-- Perform per run initialization of the build system.
function build.initialize( project_settings )
    -- Set default values for variables that can be passed on the command line.
    platform = platform or build.operating_system();
    variant = variant or "debug";
    version = version or ("%s %s %s"):format( os.date("%Y.%m.%d.%H%M"), platform, variant );
    goal = goal or "";
    jobs = jobs or 4;

    build.set_maximum_parallel_jobs( jobs );
    if build.operating_system() == "windows" then 
        build.set_build_hooks_library( build.executable("build_hooks.dll") );
    elseif build.operating_system() == "macosx" then
        build.set_build_hooks_library( build.executable("build_hooks.dylib") );
    end    

    -- Set default settings (all other settings inherit from this table).
    local default_settings = dofile( build.script("build/default_settings") );

    local local_settings = {};
    setmetatable( local_settings, {__index = default_settings}  );

    local user_settings_filename = default_settings.user_settings_filename;
    if build.exists(user_settings_filename) then
        build.merge_settings( local_settings, dofile(user_settings_filename) );
    end

    local local_settings_filename = default_settings.local_settings_filename;
    if build.exists(local_settings_filename) then
        build.merge_settings( local_settings, dofile(local_settings_filename) );
    end

    local variant_settings = default_settings.settings_by_variant[variant];
    assertf( variant_settings, "The variant '%s' is not supported", variant );

    local platform_settings = default_settings.settings_by_platform[platform];
    assertf( platform_settings, "The platform '%s' is not supported", platform );

    local settings = {};
    setmetatable( settings, {__index = local_settings} );
    build.merge_settings( settings, variant_settings );
    build.merge_settings( settings, platform_settings );
    build.merge_settings( settings, project_settings );

    if settings.library_type == "static" then
        build.Library = build.StaticLibrary;
    elseif settings.library_type == "dynamic" then
        build.Library = build.DynamicLibrary;
    else
        error( string.format("The library type '%s' is not 'static' or 'dynamic'", tostring(settings.library_type)) );
    end

    default_settings.cache = build.root( ("%s/%s_%s.cache"):format(settings.obj, platform, variant) );
    _G.settings = settings;
    build.default_settings = default_settings;
    build.local_settings = local_settings;
    build.settings = settings;
    build.configure_modules( settings );
    build.initialize_modules( settings );
    build.load_binary( settings.cache );
    build.clear();
    build.push_settings( settings );
    return settings;
end

build.modules = {};

-- Register *module* to be configured and initialized when the build sysetm 
-- is initialized.
function build.register_module( module )
    table.insert( build.modules, module ); 
end

-- Call `configure` for each registered module that provides it.
function build.configure_modules( settings )
    local modules = build.modules;
    for _, module in ipairs(modules) do 
        local configure = module.configure;
        if configure and type(configure) == "function" then 
            configure( settings );
        end
    end
end

-- Call `initialize` for each registered module that provides it.
function build.initialize_modules( settings )
    local modules = build.modules;
    for _, module in ipairs(modules) do 
        local initialize = module.initialize;
        if initialize and type(initialize) == "function" then 
            initialize( settings );
        end
    end
end

-- Convert a version string into a date table (assuming that the version 
-- string is of the form '%Y.%m.%d.%H%M').
function build.version_date( version )
    version = version or _G.version;
    local _, _, year, month, day, hour, minute = string.find( version, "(%d%d%d%d)%.(%d%d)%.(%d%d)%.(%d%d)(%d%d)" );
    return {
        year = year;
        month = month;
        day = day;
        hour = hour;
        min = min;
        sec = 0;
    };
end

-- Convert a version string into a time (assuming that the version string is
-- of the form '%Y.%m.%d.%H%M').
function build.version_time( version )
    return os.time( build.version_date(version) );
end

-- Convert a version string into the number of half days passed since 
-- *reference_time* or since 2012/01/01 00:00 if *reference_time* is not 
-- provided (assuming that the version string is of the form '%Y.%m.%d.%H%M').
function build.version_code( version, reference_time )
    reference_time = reference_time or os.time( {year = 2012; month = 1; day = 1; hour = 0; min = 0; sec = 0} );
    local SECONDS_PER_HALF_DAY = 12 * 60 * 60;
    local version_time = build.version_time( version );
    return math.ceil( os.difftime(version_time, reference_time) / SECONDS_PER_HALF_DAY );
end

-- Add targets to the current directory's target so that they will be built 
-- when a build is invoked from that directory.
function build.default_targets( targets )
    local all = build.all();
    for _, default_target in ipairs(targets) do
        local target = build.target( ("%s/all"):format(default_target) );
        all:add_dependency( target );
    end
end

-- Visit a target by calling a member function "build" if it exists and 
-- setting that Target's built flag to true if the function returns with
-- no errors.
function build.build_visit( target )
    local build_function = target.build;
    if build_function and target:outdated() then 
        local filename = target:filename();
        if filename ~= "" then
            printf( build.leaf(filename) );
        end
        target:clear_implicit_dependencies();
        local success, error_message = pcall( build_function, target );
        target:set_built( success );
        if not success then 
            build.clean_visit( target );
            assert( success, error_message );
        end
    end
end

-- Visit a target by calling a member function "clean" if it exists or if
-- there is no "clean" function and the target is not marked as a source file
-- that must exist then its associated file is deleted.
function build.clean_visit( target )
    local clean_function = target.clean;
    if clean_function then 
        clean_function( target );
    elseif target:cleanable() and target:filename() ~= "" then 
        build.rm( target:filename() );
    end
end

-- Execute command with arguments and optional filter and raise an error if 
-- it doesn't return 0.
function build.system( command, arguments, environment, dependencies_filter, stdout_filter, stderr_filter, ... )
    if type(arguments) == "table" then
        arguments = table.concat( arguments, " " );
    end
    if build.execute(command, arguments, environment, dependencies_filter, stdout_filter, stderr_filter, ...) ~= 0 then       
        error( ("%s failed"):format(arguments), 0 );
    end
end

-- Execute a command through the host system's native shell - either 
-- "C:/windows/system32/cmd.exe" on Windows system or "/bin/sh" anywhere else.
function build.shell( arguments, dependencies_filter, stdout_filter, stderr_filter, ... )
    if type(arguments) == "table" then 
        arguments = table.concat( arguments, " " );
    end
    if build.operating_system() == "windows" then
        local cmd = "C:/windows/system32/cmd.exe";
        local result = build.execute( cmd, ('cmd /c "%s"'):format(arguments), dependencies_filter, stdout_filter, stderr_filter, ... );
        assertf( result == 0, "[[%s]] failed (result=%d)", arguments, result );
    else
        local sh = "/bin/sh";
        local result = build.execute( sh, ('sh -c "%s"'):format(arguments), dependencies_filter, stdout_filter, stderr_filter, ... );
        assertf( result == 0, "[[%s]] failed (result=%d)", arguments, tonumber(result) );
    end
end

-- Return a value from a table using the first key as a lookup.
function build.switch( values )
    assert( values[1] ~= nil, "No value passed to `build.switch()`" );
    return values[values[1]];
end

-- Provide shell like string interpolation.
function build.interpolate( template, variables )
    local variables = variables or build.current_settings();
    return (template:gsub('($%b{})', function(word) return variables[word:sub(3, -2)] or word end));
end

-- Dump the keys, values, and prototype of a table for debugging.
function build.dump( t )
    print( tostring(t) );
    if t ~= nil then
        if getmetatable(t) ~= nil then
            printf( "  prototype=%s", tostring(getmetatable(t).__index) );
        end
        for k, v in pairs(t) do
            printf( "  %s -> %s", tostring(k), tostring(v) );
        end
    end
end

-- Save a settings table to a file.
function build.save_settings( settings, filename )
    -- Serialize values to to a Lua file (typically the local settings table).
    local function serialize( file, value, level )
        local function indent( level )
            for i = 1, level do
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
                indent( level + 1 );
                serialize( file, v, level + 1 );
                file:write( ",\n" );
            end
            for k, v in pairs(value) do
                if type(k) == "string" then
                    indent( level + 1 );
                    file:write( ("%s = "):format(k) );
                    serialize( file, v, level + 1 );
                    file:write( ";\n" );
                end
            end
            indent( level );
            file:write( "}" );
        end
    end

    local file = io.open( filename, "wb" );
    assertf( file, "Opening %s to write settings failed", filename );
    file:write( "\nreturn " );
    serialize( file, settings, 0 );
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

-- Merge fields with string keys from /source/ to /destination/.
function build.merge( destination, source )
    local destination = destination or {};
    for k, v in pairs(source) do
        if type(k) == "string" then
            if type(v) == "table" then
                destination[k] = build.append( destination[k], v );
            else
                destination[k] = v;
            end
        end
    end
    return destination;
end

-- Get the *all* target for the current working directory adding any 
-- targets that are passed in as dependencies.
function build.all( dependencies )
    local all = build.target( "all" );
    if dependencies then 
        build.default_depend( all, dependencies );
    end
    return all;
end

-- Find and return the initial target to build.
-- 
-- If *goal* is nil or empty then the initial target is the first all target
-- that is found in a search up from the current working directory to the
-- root directory.
--
-- Otherwise if *goal* is specified then the target that matches *goal* 
-- exactly and has at least one dependency or the target that matches 
-- `${*goal*}/all` is returned.  If neither of those targets exists then nil 
-- is returned.
function build.find_initial_target( goal )
    if not goal or goal == "" then 
        local goal = build.initial();
        local all = build.find_target( ("%s/all"):format(goal) );
        while not all and goal ~= "" do 
            goal = build.branch( goal );
            all = build.find_target( ("%s/all"):format(goal) );
        end
        return all;
    end

    local goal = build.initial( goal );
    local all = build.find_target( goal );
    if all and all:dependency() then 
        return all;
    end

    local all = build.find_target( ("%s/all"):format(goal) );
    if all and all:dependency() then
        return all;
    end
    return nil;
end

-- Save the dependency graph to the file specified by /settings.cache/.
function build.save()
    if build.local_settings.updated then
        build.local_settings.updated = nil;
        build.save_settings( build.local_settings, build.settings.local_settings_filename );
    end
    build.mkdir( build.branch(settings.cache) );
    build.save_binary( settings.cache );
end

-- Convert /name/ into a path relative to the first pattern in package.paths
-- that expands to an existing file.
function build.script( name )
    for path in string.gmatch(package.path, "([^;]*);?") do
        local filename = string.gsub( path, "?", name );
        if build.exists(filename) then
            return filename;
        end
    end
    errorf( "Script '%s' not found", filename );
end

-- Convert /filename/ into an object directory path by prepending the object 
-- directory to the portion of /filename/ that is relative to the root 
-- directory.
function build.object( filename, architecture, settings )
    settings = settings or build.current_settings();
    filename = build.relative( build.absolute(filename), build.root() );
    if architecture then 
        return ("%s/%s/%s"):format( settings.obj, architecture, filename );
    end
    return ("%s/%s"):format( settings.obj, filename );
end

-- Convert /path/ into a generated files directory path by prepending the 
-- generated directory to the portion of /path/ that is relative to the root
-- directory.
function build.generated( filename, architecture, settings )
    settings = settings or build.current_settings();
    filename = build.relative( build.absolute(filename), build.root() );
    if architecture then 
        return ("%s/%s/%s"):format( settings.gen, architecture, filename );
    end
    return ("%s/%s"):format( settings.gen, filename );
end

-- Return a string for a specific indentation level.
function build.indent( level )
    return string.rep("  ", level );
end

build.settings_stack = {};

function build.push_settings( settings )
    local settings_stack = build.settings_stack;
    local back = #settings_stack;
    if settings then
        if back > 0 then
            setmetatable( settings, {__index = settings_stack[back]} );
        end
    else
        assert( back > 0 );
        settings = settings_stack[back];
    end
    table.insert( settings_stack, settings );
    return settings;
end

function build.pop_settings()
    local settings_stack = build.settings_stack;
    assert( #settings_stack > 0 );
    table.remove( settings_stack );
end

function build.current_settings()
    local settings_stack = build.settings_stack;
    local back = #settings_stack;
    if back > 0 then
        return settings_stack[back];
    else 
        return build.settings;
    end
end

function build.store_settings()
    return #build.settings_stack;
end

function build.restore_settings( position )
    local settings_stack = build.settings_stack;
    local top_position = #settings_stack;
    while top_position > position do
        table.remove( settings_stack, top_position );
        top_position = top_position - 1;
    end 
end

-- Add dependencies detected by the injected build hooks library to the 
-- target /target/.
function build.dependencies_filter( target )
    return function( line )
        if line:match("^==") then 
            local READ_PATTERN = "^== read '([^']*)'";
            local filename = line:match( READ_PATTERN );
            if filename then
                local within_source_tree = build.relative( build.absolute(filename), build.root() ):find( "..", 1, true ) == nil;
                if within_source_tree then 
                    local header = build.ImplicitSourceFile( filename );
                    target:add_implicit_dependency( header );
                end
            end
        else
            print( line );
        end
    end
end

-- Append values from /value/ to /values/.
function build.append( values, value )
    local values = values or {};
    if type(value) == "table" then 
        for _, other_value in ipairs(value) do 
            table.insert( values, other_value );
        end
    else
        table.insert( values, value );
    end
    return values;
end

-- Recursively walk the dependencies of *target* until a target with a 
-- filename or the maximum level limit is reached.
function build.walk_dependencies( target, start, finish, maximum_level )
    local maximum_level = maximum_level or math.maxinteger;
    local function walk_dependencies_recursively( target, level )
        for _, dependency in target:dependencies() do 
            local phony = dependency:filename() == "";
            if not phony then
                coroutine.yield( dependency, level );
            end
            if phony and level + 1 < maximum_level then 
                walk_dependencies_recursively( dependency, level + 1 );
            end
        end
    end
    return coroutine.wrap( function() 
        for index, dependency in target:dependencies(start, finish) do 
            local phony = dependency:filename() == "";
            if not phony then
                coroutine.yield( dependency, level );
            end
            if phony then 
                walk_dependencies_recursively( dependency, 0 );
            end
        end
    end );
end

function build.gen_directory( target )
    return string.format( "%s/%s", target.settings.gen, build.relative(target:working_directory():path(), build.root()) );
end

function build.classes_directory( target )
    return string.format( "%s/%s", target.settings.classes, build.relative(target:working_directory():path(), build.root()) );
end

function build.obj_directory( target )
    return ("%s/%s_%s/%s"):format( target.settings.obj, platform, variant, build.relative(target:working_directory():path(), build.root()) );
end;

function build.cc_name( name )
    return ("%s.c"):format( build.basename(name) );
end;

function build.cxx_name( name )
    return ("%s.cpp"):format( build.basename(name) );
end;

function build.obj_name( name, architecture )
    return ("%s.o"):format( build.basename(name) );
end;

function build.lib_name( name )
    return ("lib%s.a"):format( name );
end;

function build.dll_name( name )
    return ("%s.dylib"):format( name );
end;

function build.exe_name( name, architecture )
    if architecture then 
        return ("%s_%s"):format( name, architecture );
    end
    return ("%s"):format( name );
end;

obj_directory = build.obj_directory;
cc_name = build.cc_name;
cxx_name = build.cxx_name;
obj_name = build.obj_name;
lib_name = build.lib_name;
dll_name = build.dll_name;
exe_name = build.exe_name;

require "build.commands";
require "build.Generate";
require "build.Directory";
require "build.Copy";
require "build.CopyDirectory";
