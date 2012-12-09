
function default()
    build.load();
    local all = all or find_target( initial(goal) );
    assert( all, "No target found at '"..tostring(initial(goal)).."'" );
    postorder( build.visit("build"), all );
    build.save();
    print( "build: default (build)=%sms" % tostring(math.ceil(ticks())) );
end

function clean()
    build.load();
    local all = all or find_target( initial(goal) );
    assert( all, "No target found at '"..tostring(initial(goal)).."'" );
    postorder( build.visit("clean"), all );
    rm( settings.cache );
    print( "build: clean=%sms" % tostring(math.ceil(ticks())) );
end

function generate()
    build.load();
    local all = all or find_target( initial(goal) );
    assert( all, "No target found at '"..tostring(initial(goal)).."'" );
    postorder( build.visit("generate"), all );
    print( "build: generate=%sms" % tostring(math.ceil(ticks())) );
end

function compile()
    assert( source, "The 'source' variable must be specified for the 'compile' command" );
    local source_file = find_target( initial(source) );
    assert( source_file, "No compilable source file found at '"..initial(source).."'" );
    local object_file = source_file.object;
    assert( object_file, "No object file found at '"..obj_directory(source_file.unit)..obj_name(source_file:id()).."'" );
    local unit = source_file.unit;
    for dependency in unit:get_dependencies() do
        if dependency:prototype() == FilePrototype and dependency ~= unit.precompiled_header then
            dependency:set_outdated( false );
        end
    end
    unit:set_outdated( true );
    object_file:set_outdated( true );
    postorder( build.visit("build"), unit );
end

function reconfigure()
    build.load();    
    rm( build.settings.local_settings_filename );
    build.load();    
    build.save();
end

function dependencies()
    build.load();
    local all = all or find_target( initial(goal) );
    assert( all, "No target found at '"..tostring(initial(goal)).."'" );
    print_dependencies( all );
end

function namespace()
    build.load();
    local all = all or find_target( initial(goal) );
    assert( all, "No target found at '"..tostring(initial(goal)).."'" );
    print_namespace( all );
end

function install()
    local platforms = build.switch { 
        operating_system();
        windows = { "msvc" };
        macosx = { "llvmgcc" };
    };

    local variants = build.switch {
        operating_system();
        windows = { "debug", "debug_dll", "release", "release_dll", "shipping", "shipping_dll" };
        macosx = { "debug", "release", "shipping" };
    };

    for _, platform in ipairs(platforms) do
        for _, variant in ipairs(variants) do
            _G.platform = platform;
            _G.variant = variant;
            default();
        end
    end
end
