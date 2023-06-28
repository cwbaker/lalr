
local paths = {
    package.path;
    root( 'src/lalr/?.lua' );
    root( 'src/lalr/?/init.lua' );
};
package.path = table.concat( paths, ';' );

variant = variant or 'debug';

local forge = require( 'forge' ):load( variant );

local cc = forge.Toolset 'cc_${platform}' {
    platform = operating_system();
    bin = root( ('%s/bin'):format(variant) );
    lib = root( ('%s/lib'):format(variant) );
    obj = root( ('%s/obj'):format(variant) );
    include_directories = {
        root( 'src' );
        root( 'src/unittest-cpp' );
    };
    library_directories = {
        root( ('%s/lib'):format(variant) );
    };
    defines = {
        ('BUILD_PLATFORM_%s'):format( upper(platform) );
        ('BUILD_VARIANT_%s'):format( upper(variant) );
        ('BUILD_VERSION="\\"%s\\""'):format( version );
    };

    assertions = variant ~= 'shipping';
    debug = variant ~= 'shipping';
    debuggable = variant ~= 'shipping';
    exceptions = true;
    fast_floating_point = variant ~= 'debug';
    incremental_linking = variant == 'debug';
    link_time_code_generation = variant == 'shipping';
    minimal_rebuild = variant == 'debug';
    optimization = variant ~= 'debug';
    run_time_checks = variant == 'debug';
    runtime_library = variant == 'debug' and 'static_debug' or 'static_release';
    run_time_type_info = true;
    stack_size = 1048576;
    standard = 'c++14';
    string_pooling = variant == 'shipping';
    strip = false;
    warning_level = 3;
    warnings_as_errors = true;
};

cc:install( 'forge.cc' );

-- Bump the C++ standard to c++14 when building on Windows as that is the
-- lowest standard supported by Microsoft Visual C++.
--if cc.platform == 'windows' then
    cc.standard = 'c++17';
--end

local lalr = require 'forge.lalr';
cc:install( lalr );

buildfile 'src/lalr/lalr.forge';
buildfile 'src/unittest-cpp/unittest-cpp.forge';

cc:all {
    'src/lalr/all',
    'src/lalr/lalrc/all',
    'src/lalr/lalr_examples/all',
    'src/lalr/lalr_test/all'
};
