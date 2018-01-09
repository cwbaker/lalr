
clang = {};

function clang.append_defines( target, defines )
    table.insert( defines, ('-DBUILD_PLATFORM_%s'):format(build.upper(platform)) );
    table.insert( defines, ('-DBUILD_VARIANT_%s'):format(build.upper(variant)) );
    table.insert( defines, ('-DBUILD_LIBRARY_TYPE_%s'):format(build.upper(target.settings.library_type)) );
    table.insert( defines, ('-DBUILD_BIN_DIRECTORY="\\"%s\\""'):format(target.settings.bin) );
    table.insert( defines, ('-DBUILD_MODULE_DIRECTORY="\\"%s\\""'):format(target:working_directory():path()) );
    table.insert( defines, ('-DBUILD_VERSION="\\"%s\\""'):format(version) );

    if string.find(target.settings.runtime_library, "debug", 1, true) then
        table.insert( defines, "-D_DEBUG" );
        table.insert( defines, "-DDEBUG" );
    else 
        table.insert( defines, "-DNDEBUG" );
    end

    if target.settings.defines then
        for _, define in ipairs(target.settings.defines) do
            table.insert( defines, ("-D%s"):format(define) );
        end
    end
    
    if target.defines then
        for _, define in ipairs(target.defines) do
            table.insert( defines, ("-D%s"):format(define) );
        end
    end
end

function clang.append_include_directories( target, include_directories )
    if target.include_directories then
        for _, directory in ipairs(target.include_directories) do
            table.insert( include_directories, ('-I "%s"'):format(relative(directory)) );
        end
    end

    if target.settings.include_directories then
        for _, directory in ipairs(target.settings.include_directories) do
            table.insert( include_directories, ('-I "%s"'):format(directory) );
        end
    end

    if target.framework_directories then 
        for _, directory in ipairs(target.framework_directories) do
            table.insert( include_directories, ('-F "%s"'):format(directory) );
        end
    end

    if target.settings.framework_directories then 
        for _, directory in ipairs(target.settings.framework_directories) do
            table.insert( include_directories, ('-F "%s"'):format(directory) );
        end
    end
end

function clang.append_compile_flags( target, flags )
    table.insert( flags, "-c" );
    table.insert( flags, ("-arch %s"):format(target.architecture) );
    table.insert( flags, "-fasm-blocks" );
    
    local language = target.language or "c++";
    if language then
        table.insert( flags, ("-x %s"):format(language) );
        if string.find(language, "c++", 1, true) then
            table.insert( flags, "-std=c++11" );
            table.insert( flags, "-stdlib=libc++" );
            table.insert( flags, "-Wno-deprecated" );
            if target.settings.exceptions then
                table.insert( flags, "-fexceptions" );
            end
            if target.settings.run_time_type_info then
                table.insert( flags, "-frtti" );
            end
        end

        if string.find(language, "objective", 1, true) then
            table.insert( flags, "-fobjc-abi-version=2" );
            table.insert( flags, "-fobjc-legacy-dispatch" );
            table.insert( flags, '"-DIBOutlet=__attribute__((iboutlet))"' );
            table.insert( flags, '"-DIBOutletCollection(ClassName)=__attribute__((iboutletcollection(ClassName)))"' );
            table.insert( flags, '"-DIBAction=void)__attribute__((ibaction)"' );
            if target.settings.objc_arc then
                table.insert( flags, "-fobjc-arc" );
            end
            if target.settings.objc_modules then
                if language == "objective-c" then
                    table.insert( flags, "-fmodules" );
                end
            end
        end
    end
        
    if target.settings.debug then
        table.insert( flags, "-g3" );
    end

    if target.settings.optimization then
        table.insert( flags, "-O3" );
        table.insert( flags, "-Ofast" );
    end
    
    if target.settings.preprocess then
        table.insert( flags, "-E" );
    end

    if target.settings.runtime_checks then
        table.insert( flags, "-fstack-protector" );
    else
        table.insert( flags, "-fno-stack-protector" );
    end

    if target.settings.warnings_as_errors then 
        table.insert( flags, "-Werror" );
    end

    local warning_level = target.settings.warning_level
    if warning_level == 0 then 
        table.insert( flags, "-w" );
    elseif warning_level == 1 then
        table.insert( flags, "-Wall" );
    elseif warning_level == 2 then
        table.insert( flags, "-Wall -Wextra" );
    elseif warning_level == 3 then
        table.insert( flags, "-Wall -Wextra" );
    else
        table.insert( flags, "-Wall -Wextra -Weverything" );
    end
end

function clang.append_library_directories( target, library_directories )
    if target.library_directories then
        for _, directory in ipairs(target.library_directories) do
            table.insert( library_directories, ('-L "%s"'):format(directory) );
        end
    end
    
    if target.settings.library_directories then
        for _, directory in ipairs(target.settings.library_directories) do
            table.insert( library_directories, ('-L "%s"'):format(directory) );
        end
    end
    
    if target.framework_directories then 
        for _, directory in ipairs(target.framework_directories) do
            table.insert( library_directories, ('-F "%s"'):format(directory) );
        end
    end
    
    if target.settings.framework_directories then 
        for _, directory in ipairs(target.settings.framework_directories) do
            table.insert( library_directories, ('-F "%s"'):format(directory) );
        end
    end
end

function clang.append_link_flags( target, flags )
    table.insert( flags, ("-arch %s"):format(target.architecture) );
    table.insert( flags, "-std=c++11" );
    table.insert( flags, "-stdlib=libc++" );

    if target:prototype() == build.DynamicLibrary then
        table.insert( flags, "-Xlinker -dylib" );
    end
    
    if target.settings.verbose_linking then
        table.insert( flags, "-Wl,--verbose=31" );
    end
    
    if target.settings.generate_map_file then
        table.insert( flags, ('-Wl,-map,"%s"'):format(build.native(("%s/%s.map"):format(obj_directory(target), target:id()))) );
    end

    if target.settings.strip and not target.settings.generate_dsym_bundle then
        table.insert( flags, "-Wl,-dead_strip" );
    end

    if target.settings.exported_symbols_list then
        table.insert( flags, ('-exported_symbols_list "%s"'):format(absolute(target.settings.exported_symbols_list)) );
    end

    table.insert( flags, ('-o "%s"'):format(build.native(target:filename())) );
end

function clang.append_link_libraries( target, libraries )
    if target.settings.third_party_libraries then
        for _, library in ipairs(target.settings.third_party_libraries) do
            table.insert( libraries, ("-l%s"):format(library) );
        end
    end

    if target.third_party_libraries then
        for _, library in ipairs(target.third_party_libraries) do
            table.insert( libraries, ("-l%s"):format(library) );
        end
    end

    if target.system_libraries then
        for _, library in ipairs(target.system_libraries) do 
            table.insert( libraries, ("-l%s"):format(library) );
        end
    end

    if target.frameworks then
        for _, framework in ipairs(target.frameworks) do
            table.insert( libraries, ('-framework "%s"'):format(framework) );
        end
    end

    if target.settings.frameworks then 
        for _, framework in ipairs(target.settings.frameworks) do
            table.insert( libraries, ('-framework "%s"'):format(framework) );
        end
    end
end

function clang.parse_dependencies_file( filename, object )
    object:clear_implicit_dependencies();

    local file = io.open( filename, "r" );
    assertf( file, "Opening '%s' to parse dependencies failed", filename );
    local dependencies = file:read( "a" );
    file:close();
    file = nil;

    local TARGET_PATTERN = "([^:]+):[ \t\n\r\\]+";
    local DEPENDENCY_PATTERN = "([^\n\r]+) \\[ \t\n\r]+";
    local start, finish, path = dependencies:find( TARGET_PATTERN );
    if start and finish then 
        local start, finish, path = dependencies:find( DEPENDENCY_PATTERN, finish + 1 );
        while start and finish do 
            local filename = path:gsub( "\\ ", " " );
            local within_source_tree = build.relative( build.absolute(filename), build.root() ):find( "..", 1, true ) == nil;
            if within_source_tree then 
                local dependency = build.ImplicitSourceFile( path:gsub("\\ ", " ") );
                object:add_implicit_dependency( dependency );
            end
            start, finish, path = dependencies:find( DEPENDENCY_PATTERN, finish + 1 );
        end
    end
end

build.register_module( clang );
