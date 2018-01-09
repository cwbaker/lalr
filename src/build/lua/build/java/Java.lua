
local Java = build.TargetPrototype( "Java" );

function Java.create( settings, definition )
    local java_ = build.Target( build.anonymous(), Java, definition );
    java_.settings = settings;
    java_:set_filename( ("%s/Java.%s.timestamp"):format(build.classes_directory(java_), java_:id()) );
    java_:add_ordering_dependency( build.Directory(build.classes_directory(java_)) );

    build.pushd( java_.sourcepath or "." );
    for _, value in ipairs(definition) do
        if type(value) == "string" then
            local source = build.file( value );
            source:set_required_to_exist( true );
            java_:add_dependency( source );
        elseif type(value) == "table" then
            java_:add_dependency( value );
        end
    end
    build.popd();

    java.add_jar_dependencies( java_, java_.settings.jars );
    java.add_jar_dependencies( java_, definition.jars );

    return java_;
end

function Java.build( java )
    local jars = {};
    local third_party_jars = java.third_party_jars;
    if third_party_jars then 
        for _, jar in ipairs(third_party_jars) do 
            table.insert( jars, jar );
        end
    end
    local third_party_jars = java.settings.third_party_jars;
    if third_party_jars then 
        for _, jar in ipairs(third_party_jars) do 
            table.insert( jars, jar );
        end
    end

    local source_files = {};
    for _, dependency in java:dependencies() do 
        local prototype = dependency:prototype();
        if prototype == nil then
            table.insert( source_files, build.relative(dependency:filename()) );
        elseif prototype == build.Jar then
            table.insert( jars, build.relative(dependency:filename()) );
        end
    end

    if #source_files > 0 then
        local settings = java.settings;
        local javac = ("%s/bin/javac"):format( settings.java.jdk_directory );
        local output = build.classes_directory( java );
        local classpath = output;

        local sourcepaths = {};
        if java.sourcepaths then
            for _, path in ipairs(java.sourcepaths) do 
                table.insert( sourcepaths, path );
            end
        end
        if settings.sourcepaths then 
            for _, path in ipairs(java.settings.sourcepaths) do 
                table.insert( sourcepaths, path );
            end
        end
        table.insert( sourcepaths, java.sourcepath or "." );
        table.insert( sourcepaths, build.gen_directory(java) );
        
        local command_line = {
            'javac',
            '-Xlint:unchecked',
            ('-d "%s"'):format(output),
            ('-sourcepath "%s"'):format(table.concat(sourcepaths, ':')),
            ('-classpath "%s"'):format(classpath),
            '-target 1.7',
            ('-bootclasspath "%s"'):format(table.concat(jars, ':')),
            '-encoding UTF-8',
            '-g',
            '-source 1.7',
            ('%s'):format(table.concat(source_files, ' '))
        };

        build.system( javac, command_line );

        local timestamp_file = io.open( java:filename(), "wb" );
        assertf( timestamp_file, "Opening '%s' to write generated text failed", java:filename() );
        timestamp_file:write( ("# Timestamp for '%s'"):format(java:path()) );
        timestamp_file:close();
        timestamp_file = nil;
    end
end

function Java.clean( java )
    build.rm( java:filename() );
    build.rmdir( build.classes_directory(java) )
end

java.Java = Java;
