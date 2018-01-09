
local Java = build:TargetPrototype( "Java" );

function Java.create( settings )
    local java_ = build:Target( build:anonymous(), Java );
    java_.settings = settings;
    java_:set_filename( ("%s/Java.%s.timestamp"):format(settings.classes_directory(java_), java_:id()) );
    java_:add_ordering_dependency( build:Directory(settings.classes_directory(java_)) );
    java.add_jar_dependencies( java_, settings.jars );
    return java_;
end

function Java:depend( dependencies )
    build:pushd( dependencies.sourcepath or "." );
    for _, value in ipairs(dependencies) do
        local source = build:SourceFile( value, settings );
        self:add_dependency( source );
    end
    build:popd();

    local jars = dependencies.jars;
    if jars then 
        java.add_jar_dependencies( self, jars );
        dependencies.jars = nil;
    end

    build:merge( self, dependencies );
    return self
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
            table.insert( source_files, build:relative(dependency:filename()) );
        elseif prototype == build.Jar then
            table.insert( jars, build:relative(dependency:filename()) );
        end
    end

    if #source_files > 0 then
        local settings = java.settings;
        local javac = ("%s/bin/javac"):format( settings.java.jdk_directory );
        local output = settings.classes_directory( java );
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
        table.insert( sourcepaths, settings.gen_directory(java) );
        
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
        build:system( javac, command_line );

        local timestamp_file = io.open( java:filename(), "wb" );
        assertf( timestamp_file, "Opening '%s' to write generated text failed", java:filename() );
        timestamp_file:write( ("# Timestamp for '%s'"):format(java:path()) );
        timestamp_file:close();
        timestamp_file = nil;
    end
end

function Java.clean( java )
    build:rm( java:filename() );
    build:rmdir( settings.classes_directory(java) )
end

java.Java = Java;
