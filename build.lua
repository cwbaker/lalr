
local paths = {
    package.path;
    build:root( 'lalr/?.lua' );
    build:root( 'lalr/?/init.lua' );
};
package.path = table.concat( paths, ';' );

require 'build';
require 'build.cc';
require 'build.lalr';
require 'build.visual_studio';
require 'build.xcode';
require 'build.macos';
require 'build.windows';

platform = platform or build:operating_system();
variant = variant or 'debug';
version = version or ('%s %s %s'):format(os.date('%Y.%m.%d %H:%M:%S'), platform, variant );
goal = goal or "";
jobs = jobs or 4;

build:initialize {
    variants = { 'debug', 'release', 'shipping' };
    bin = build:root( ('%s/bin'):format(variant) );
    lib = build:root( ('%s/lib'):format(variant) );
    obj = build:root( ('%s/obj'):format(variant) );
    include_directories = {
        build:root( 'lalr' );
        build:root( 'unittest-cpp' );
    };
    library_directories = {
        build:root( ('%s/lib'):format(variant) );
    };
    visual_studio = {
        sln = build:root( 'lalr.sln' );
    };
    xcode = {
        xcodeproj = build:root( 'lalr.xcodeproj' );
    };
    lalr = {
        lalrc = build:switch {
            build:operating_system();
            macos = build:root( ('%s/bin/lalrc'):format(variant) );
            windows = build:root( ('%s/bin/lalrc.exe'):format(variant) );
        };
    };
};

build:default_targets {
    'lalr/lalrc',
    'lalr/lalr_examples',
    'lalr/lalr_test'
};

buildfile 'lalr/lalr.build';
buildfile 'unittest-cpp/unittest-cpp.build';
