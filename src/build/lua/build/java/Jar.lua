
local Jar = build:TargetPrototype( "Jar" );

local function default_filename( identifier, settings )
    local settings = settings or build:current_settings();
    local branch = settings.lib;
    if build:is_absolute(identifier) then 
        branch = build:branch( identifier );
    end
    return ("%s/%s.jar"):format( branch, build:basename(identifier) );
end

function Jar.create( settings, identifier )
    local jar = build:Target( identifier, Jar );
    jar:set_filename( default_filename(identifier, settings) );
    jar.settings = settings;
    return jar;
end

function Jar.depend( jar, dependencies )
    local jars = dependencies.jars;
    if jars then 
        java.add_jar_dependencies( java, dependencies.jars );
        dependencies.jars = nil;
    end
    return build.Target.depend( jar, dependencies );
end

local function included( jar, filename )
    if build:is_directory(filename) then 
        return false;
    end

    if jar.excludes then 
        for _, pattern in ipairs(jar.excludes) do 
            if string.match(filename, pattern) then 
                return false;
            end
        end
    end

    if jar.includes then 
        for _, pattern in ipairs(jar.includes) do 
            if string.match(filename, pattern) then 
                return true;
            end
        end
        return false;
    end

    return true;
end

function Jar.build( jar )
    local settings = jar.settings;
    local jar_ = build:native( ("%s/bin/jar"):format(jar.settings.java.jdk_directory) );
    local directory = settings.classes_directory( jar );
    build:pushd( directory );

    local classes = {};
    for filename in build:find(".") do 
        if included(jar, filename) then
            table.insert( classes, build:relative(filename) );
        end
    end

    build:system( jar_, ('jar cvf "%s" "%s"'):format(jar:filename(), table.concat(classes, [[" "]])) );
    build:popd();
end

java.Jar = Jar;
