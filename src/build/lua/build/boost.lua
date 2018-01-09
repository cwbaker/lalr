
local boost = {};

function boost.configure( settings )
    local local_settings = build.local_settings;
    if not local_settings.boost then
        local_settings.updated = true;
        local_settings.boost = {
            boost_directory = "";
            version = "1.56";
        };
        if operating_system() == "windows" then
            local_settings.boost.boost_directory = "C:/local";
        else
            local_settings.boost.boost_directory = home( "boost" );
        end
    end
end

function boost.initialize( settings )
    boost.configure(settings);
    local boost_library_by_platform = {
        [""] = boost.default_boost_library;
        llvmgcc = boost.llvmgcc_boost_library;
        msvc = boost.msvc_boost_library;
        mingw = boost.mingw_boost_library;
    };
    assert( boost_library_by_platform[platform], ("No boost_library() implementation for the platform '%s'!"):format(platform) );
    boost_library = boost_library_by_platform[platform];
end

function boost.default_boost_library( name )
    return name;
end

function boost.msvc_boost_library( name )
    local toolset = "vc120";
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

    return ("lib%s-%s-%s-%s.lib"):format( name, toolset, runtime, version );
end

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

    return ("%s-%s-%s-%s"):format( name, toolset, runtime, version );
end

function boost.llvmgcc_boost_library( name )
    local toolset = "xgcc42";
    local runtime = "";
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

    return ("%s-%s-%s-%s"):format( name, toolset, runtime, version );
end

_G.boost = boost;
return boost;
