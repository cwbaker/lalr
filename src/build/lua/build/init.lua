
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

-- Provide buildfile() that restores the settings stack position.
local original_buildfile = build.buildfile;
function buildfile( ... )
    local buildfiles_stack = build.buildfiles_stack_;
    table.insert( buildfiles_stack, build:file(select(1, ...)) );
    local success, errors_or_error_message = pcall( original_buildfile, build, ... );
    table.remove( buildfiles_stack );
    assertf( success and errors_or_error_message == 0, "Loading buildfile '%s' failed", tostring(select(1, ...)) );
end

build = _G.build or {};
build.settings = {};
build.buildfiles_stack_ = {};
build.modules_ = {};
build.default_builds_ = {};

function build:default_build( identifier, build )
    table.insert( self.default_builds_, {identifier, build} );
end

function build:default_builds( pattern )
    return coroutine.wrap( function() 
        local index = 1;
        for _, default_build in ipairs(self.default_builds_) do 
            local identifier = default_build[1];
            if pattern == nil or pattern == "" or identifier:find(pattern) then 
                coroutine.yield( index, default_build[2] );
                index = index + 1;
            end
        end        
    end );
end

function build:current_buildfile()
    local buildfiles_stack = self.buildfiles_stack_;
    local back = #buildfiles_stack;
    if back > 0 then 
        return buildfiles_stack[back];
    end
end

function build:platform_matches( ... )
    local platform = self.settings.platform;
    if platform == "" then 
        return true;
    end
    for i = 1, select("#", ...) do
        if platform:match(select(i, ...)) then
            return true;
        end
    end
    return false;
end

function build:File( filename, target_prototype )
    local target = self:Target( self:interpolate(filename), target_prototype );
    target:set_filename( target:path() );
    target:set_cleanable( true );
    return target;
end

function build:SourceFile( value, settings )
    local target = value;
    if type(target) == "string" then 
        target = self:Target( build:interpolate(value, settings) );
        target:set_filename( target:path() );
        target:set_cleanable( false );
    end
    return target;
end

function build:SourceDirectory( value, settings )
    return self:SourceFile( value, settings );
end

function build:map( target_prototype, replacement, pattern, filenames )
    local targets = {};
    local settings = settings or self:current_settings();
    for _, source_filename in ipairs(filenames) do 
        local source = build:relative( source_filename );
        local filename, substitutions = source:gsub( pattern, replacement );
        if substitutions > 0 then 
            local destination = build:interpolate( filename, settings );
            local target = target_prototype (self, destination) (source);
            table.insert( targets, target );
        end
    end
    return table.unpack( targets );
end

function build:map_ls( target_prototype, replacement, pattern, settings )
    local targets = {};
    local settings = settings or self:current_settings();
    for source_filename in build:ls("") do
        local source = build:relative( source_filename );
        local filename, substitutions = source:gsub( pattern, replacement );
        if substitutions > 0 then    
            local destination = build:interpolate( filename, settings );
            local target = target_prototype (self, destination) (source);
            table.insert( targets, target );
        end
    end
    return table.unpack( targets );
end

function build:map_find( target_prototype, replacement, pattern, settings )
    local targets = {};
    local settings = settings or self:current_settings();
    for source_filename in build:find("") do
        if build:is_file(source_filename) then
            local source = build:relative( source_filename );
            local filename, substitutions = source:gsub( pattern, replacement );
            if substitutions > 0 then
                local destination = build:interpolate( filename, settings );
                local target = target_prototype (self, destination) (source);
                table.insert( targets, target );
            end
        end
    end
    return table.unpack( targets );
end

-- Perform per run initialization of the build system.
function build:initialize( project_settings )
    -- Merge settings from /source_settings/ into /settings/.
    local function merge_settings( settings, source_settings )
        settings = settings or {};
        for _, v in ipairs(source_settings) do
            table.insert( settings, v );
        end
        for k, v in pairs(source_settings) do
            if type(k) == "string" then
                if type(v) == "table" then
                    settings[k] = merge_settings( settings[k], v );
                else
                    settings[k] = v;
                end
            end
        end
        return settings;
    end

    -- Set default values for variables that can be passed on the command line.
    platform = platform or self:operating_system();
    variant = variant or "debug";
    version = version or ("%s %s %s"):format( os.date("%Y.%m.%d.%H%M"), platform, variant );
    goal = goal or "";
    jobs = jobs or 4;

    self:set_maximum_parallel_jobs( jobs );
    if self:operating_system() == "windows" then 
        self:set_build_hooks_library( self:executable("build_hooks.dll") );
    elseif self:operating_system() == "macosx" then
        self:set_build_hooks_library( self:executable("build_hooks.dylib") );
    end    

    -- Set default settings (all other settings inherit from this table).
    local default_settings = dofile( self:script("build/default_settings") );

    local local_settings = {};
    setmetatable( local_settings, {__index = default_settings}  );

    local user_settings_filename = default_settings.user_settings_filename;
    if self:exists(user_settings_filename) then
        merge_settings( local_settings, dofile(user_settings_filename) );
    end

    local local_settings_filename = default_settings.local_settings_filename;
    if self:exists(local_settings_filename) then
        merge_settings( local_settings, dofile(local_settings_filename) );
    end

    local variant_settings = default_settings.settings_by_variant[variant];
    assertf( variant_settings, "The variant '%s' is not supported", variant );

    local platform_settings = default_settings.settings_by_platform[platform];
    assertf( platform_settings, "The platform '%s' is not supported", platform );

    local settings = {};
    setmetatable( settings, {__index = local_settings} );
    merge_settings( settings, variant_settings );
    merge_settings( settings, platform_settings );
    merge_settings( settings, project_settings );

    if settings.library_type == "static" then
        self.Library = self.StaticLibrary;
    elseif settings.library_type == "dynamic" then
        self.Library = self.DynamicLibrary;
    else
        error( string.format("The library type '%s' is not 'static' or 'dynamic'", tostring(settings.library_type)) );
    end

    default_settings.cache = self:root( ("%s/%s.cache"):format(settings.obj, variant) );
    _G.settings = settings;
    self.default_settings = default_settings;
    self.local_settings = local_settings;
    self.settings = settings;
    self:configure_modules( settings );
    self:initialize_modules( settings );
    self:load_binary( settings.cache );
    self:clear();
    return settings;
end

-- Register *module* to be configured and initialized when the build sysetm 
-- is initialized.
function build:register_module( module )
    table.insert( self.modules_, module ); 
end

-- Call `configure` for each registered module that provides it.
function build:configure_modules( settings )
    local modules = self.modules_;
    for _, module in ipairs(modules) do 
        local configure = module.configure;
        if configure and type(configure) == "function" then 
            configure( settings );
        end
    end
end

-- Call `initialize` for each registered module that provides it.
function build:initialize_modules( settings )
    local modules = self.modules_;
    for _, module in ipairs(modules) do 
        local initialize = module.initialize;
        if initialize and type(initialize) == "function" then 
            initialize( settings );
        end
    end
end

-- Convert a version string into a date table (assuming that the version 
-- string is of the form '%Y.%m.%d.%H%M').
function build:version_date( version )
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
function build:version_time( version )
    return os.time( self:version_date(version) );
end

-- Convert a version string into the number of half days passed since 
-- *reference_time* or since 2012/01/01 00:00 if *reference_time* is not 
-- provided (assuming that the version string is of the form '%Y.%m.%d.%H%M').
function build:version_code( version, reference_time )
    reference_time = reference_time or os.time( {year = 2012; month = 1; day = 1; hour = 0; min = 0; sec = 0} );
    local SECONDS_PER_HALF_DAY = 12 * 60 * 60;
    local version_time = self:version_time( version );
    return math.ceil( os.difftime(version_time, reference_time) / SECONDS_PER_HALF_DAY );
end

-- Add targets to the current directory's target so that they will be built 
-- when a build is invoked from that directory.
function build:default_targets( targets )
    local all = build:all();
    for _, default_target in ipairs(targets) do
        local target = build:target( ("%s/all"):format(default_target) );
        all:add_dependency( target );
    end
end

-- Execute command with arguments and optional filter and raise an error if 
-- it doesn't return 0.
function build:system( command, arguments, environment, dependencies_filter, stdout_filter, stderr_filter, ... )
    if type(arguments) == "table" then
        arguments = table.concat( arguments, " " );
    end
    if self:execute(command, arguments, environment, dependencies_filter, stdout_filter, stderr_filter, ...) ~= 0 then       
        error( ("%s failed"):format(arguments), 0 );
    end
end

-- Execute a command through the host system's native shell - either 
-- "C:/windows/system32/cmd.exe" on Windows system or "/bin/sh" anywhere else.
function build:shell( arguments, dependencies_filter, stdout_filter, stderr_filter, ... )
    if type(arguments) == "table" then 
        arguments = table.concat( arguments, " " );
    end
    if self:operating_system() == "windows" then
        local cmd = "C:/windows/system32/cmd.exe";
        local result = self:execute( cmd, ('cmd /c "%s"'):format(arguments), dependencies_filter, stdout_filter, stderr_filter, ... );
        assertf( result == 0, "[[%s]] failed (result=%d)", arguments, result );
    else
        local sh = "/bin/sh";
        local result = self:execute( sh, ('sh -c "%s"'):format(arguments), dependencies_filter, stdout_filter, stderr_filter, ... );
        assertf( result == 0, "[[%s]] failed (result=%d)", arguments, tonumber(result) );
    end
end

-- Return a value from a table using the first key as a lookup.
function build:switch( values )
    assert( values[1] ~= nil, "No value passed to `build:switch()`" );
    return values[values[1]];
end

-- Provide shell like string interpolation.
function build:interpolate( template, variables )
    local variables = variables or self:current_settings();
    return (template:gsub('($%b{})', function(word) return variables[word:sub(3, -2)] or word end));
end

-- Dump the keys, values, and prototype of a table for debugging.
function build:dump( t )
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
function build:save_settings( settings, filename )
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

-- Merge fields with string keys from /source/ to /destination/.
function build:merge( destination, source )
    local destination = destination or {};
    for k, v in pairs(source) do
        if type(k) == "string" then
            if type(v) == "table" then
                destination[k] = build:append( destination[k], v );
            else
                destination[k] = v;
            end
        end
    end
    return destination;
end

-- Get the *all* target for the current working directory adding any 
-- targets that are passed in as dependencies.
function build:all( dependencies )
    local all = self:target( "all" );
    if dependencies then 
        all:depend( dependencies );
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
function build:find_initial_target( goal )
    if not goal or goal == "" then 
        local goal = self:initial();
        local all = self:find_target( ("%s/all"):format(goal) );
        while not all and goal ~= "" do 
            goal = self:branch( goal );
            all = self:find_target( ("%s/all"):format(goal) );
        end
        return all;
    end

    local goal = self:initial( goal );
    local all = self:find_target( goal );
    if all and all:dependency() then 
        return all;
    end

    local all = self:find_target( ("%s/all"):format(goal) );
    if all and all:dependency() then
        return all;
    end
    return nil;
end

-- Save the dependency graph to the file specified by /settings.cache/.
function build:save()
    local settings = self:current_settings();
    if self.local_settings.updated then
        self.local_settings.updated = nil;
        self:save_settings( self.local_settings, settings.local_settings_filename );
    end
    self:mkdir( self:branch(settings.cache) );
    self:save_binary( settings.cache );
end

-- Convert /name/ into a path relative to the first pattern in package.paths
-- that expands to an existing file.
function build:script( name )
    for path in string.gmatch(package.path, "([^;]*);?") do
        local filename = string.gsub( path, "?", name );
        if self:exists(filename) then
            return filename;
        end
    end
    errorf( "Script '%s' not found", filename );
end

-- Convert /filename/ into an object directory path by prepending the object 
-- directory to the portion of /filename/ that is relative to the root 
-- directory.
function build:object( filename, architecture, settings )
    settings = settings or self:current_settings();
    filename = self:relative( self:absolute(filename), self:root() );
    if architecture then 
        return ("%s/%s/%s"):format( settings.obj, architecture, filename );
    end
    return ("%s/%s"):format( settings.obj, filename );
end

-- Convert /path/ into a generated files directory path by prepending the 
-- generated directory to the portion of /path/ that is relative to the root
-- directory.
function build:generated( filename, architecture, settings )
    settings = settings or self:current_settings();
    filename = self:relative( self:absolute(filename), self:root() );
    if architecture then 
        return ("%s/%s/%s"):format( settings.gen, architecture, filename );
    end
    return ("%s/%s"):format( settings.gen, filename );
end

function build:configure( settings )
    local build = { 
        __this = self.__this;
        settings = settings;
    };
    setmetatable( settings, {
        __index = self.settings;
    } );
    setmetatable( build, {
        __index = self;
    } );
    return build, settings;
end

function build:current_settings()
    return self.settings;
end

-- Add dependencies detected by the injected build hooks library to the 
-- target /target/.
function build:dependencies_filter( target )
    return function( line )
        if line:match("^==") then 
            local READ_PATTERN = "^== read '([^']*)'";
            local filename = line:match( READ_PATTERN );
            if filename then
                local within_source_tree = self:relative( self:absolute(filename), self:root() ):find( "..", 1, true ) == nil;
                if within_source_tree then 
                    local header = self:SourceFile( filename );
                    target:add_implicit_dependency( header );
                end
            end
        else
            print( line );
        end
    end
end

-- Append values from /value/ to /values/.
function build:append( values, value )
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
function build:walk_dependencies( target, start, finish, maximum_level )
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

require "build.commands";
require "build.TargetPrototype";
require "build.Target";
require "build.Generate";
require "build.Directory";
require "build.Copy";
require "build.CopyDirectory";
