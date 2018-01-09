
function default()
    local failures = build.postorder( build.build_visit, build.find_initial_target() );
    build.save();
    printf( "build: default (build)=%dms", math.ceil(build.ticks()) );
    return failures;
end

function clean()
    local failures = build.postorder( build.clean_visit, build.find_initial_target() );
    printf( "build: clean=%sms", tostring(math.ceil(build.ticks())) );
    return failures;
end

function reconfigure()
    rm( build.settings.local_settings_filename );
    build.find_initial_target();
    return 0;
end

function dependencies()
    build.print_dependencies( build.find_initial_target() );
    return 0;
end

function namespace()
    build.print_namespace( build.find_initial_target() );
    return 0;
end
