
boost = {};

function boost.configure( settings )
    local local_settings = build.local_settings;
    if not local_settings.boost then
        local_settings.updated = true;
        local_settings.boost = {
            boost_directory = "";
            version = "1.43";
        };
        if operating_system() == "windows" then
            local_settings.boost.boost_directory = "C:/boost";
        else
            local_settings.boost.boost_directory = home( "boost" );
        end
    end
end;

function boost.initialize( settings )
    boost.configure(settings);
    local boost_library_by_platform = {
        llvmgcc = boost.llvmgcc_boost_library;
        msvc = boost.msvc_boost_library;
        mingw = boost.mingw_boost_library;
    };
    assert( boost_library_by_platform[platform], "No boost_library() implementation for the platform '%s'!" % platform );
    boost_library = boost_library_by_platform[platform];
end;

function boost.msvc_boost_library( name )
    local toolset = "vc90";
    local runtime;
    local version = string.gsub( settings.boost.version, "%.", "_" );

    if settings.runtime_library == "static" then
        runtime = "mt-s";
    elseif settings.runtime_library == "static_debug" then
        runtime = "mt-sgd";
    elseif settings.runtime_library == "dynamic" then
        runtime = "mt";
    elseif settings.runtime_library == "dynamic_debug" then
        runtime = "mt-gd";
    end

    return "lib%s-%s-%s-%s.lib" % { name, toolset, runtime, version };
end;

function boost.mingw_boost_library( name )
    local toolset = "mgw46";
    local runtime;
    local version = string.gsub( settings.boost.version, "%.", "_" );

    if settings.runtime_library == "static" then
        runtime = "mt-s";
    elseif settings.runtime_library == "static_debug" then
        runtime = "mt-sd";
    elseif settings.runtime_library == "dynamic" then
        runtime = "mt";
    elseif settings.runtime_library == "dynamic_debug" then
        runtime = "mt-d";
    end

    return "%s-%s-%s-%s" % { name, toolset, runtime, version };
end;

function boost.llvmgcc_boost_library( name )
    local toolset = "xgcc42";
    local runtime;
    local version = string.gsub( settings.boost.version, "%.", "_" );

    if settings.runtime_library == "static" then
        runtime = "mt-s";
    elseif settings.runtime_library == "static_debug" then
        runtime = "mt-sd";
    elseif settings.runtime_library == "dynamic" then
        runtime = "mt";
    elseif settings.runtime_library == "dynamic_debug" then
        runtime = "mt-d";
    end

    return "%s-%s-%s-%s" % { name, toolset, runtime, version };    
end;
