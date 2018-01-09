
-- Visit a target by calling a member function "clean" if it exists or if
-- there is no "clean" function and the target is not marked as a source file
-- that must exist then its associated file is deleted.
local function clean_visit( target )
    local clean_function = target.clean;
    if clean_function then 
        clean_function( target );
    elseif target:cleanable() and target:filename() ~= "" then 
        build:rm( target:filename() );
    end
end

-- Visit a target by calling a member function "build" if it exists and 
-- setting that Target's built flag to true if the function returns with
-- no errors.
local function build_visit( target )
    local build_function = target.build;
    if build_function and target:outdated() then 
        local filename = target:filename();
        if filename ~= "" then
            printf( build:leaf(filename) );
        end
        target:clear_implicit_dependencies();
        local success, error_message = pcall( build_function, target );
        target:set_built( success );
        if not success then 
            clean_visit( target );
            assert( success, error_message );
        end
    end
end

function default()
    local failures = build:postorder( build_visit, build:find_initial_target() );
    build:save();
    printf( "build: default (build)=%dms", math.ceil(build:ticks()) );
    return failures;
end

function clean()
    local failures = build:postorder( clean_visit, build:find_initial_target() );
    printf( "build: clean=%sms", tostring(math.ceil(build:ticks())) );
    return failures;
end

function reconfigure()
    rm( build.settings.local_settings_filename );
    build:find_initial_target();
    return 0;
end

function dependencies()
    build:print_dependencies( build:find_initial_target() );
    return 0;
end

function namespace()
    build:print_namespace( build:find_initial_target() );
    return 0;
end
