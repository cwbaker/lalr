
require "build";
require "build.cc";
require "build.parser";
require "build.visual_studio";
require "build.xcode";
require "build.macos";
require "build.windows";

platform = platform or build:operating_system();
variant = variant or "debug";
version = version or ("%s %s %s"):format(os.date("%Y.%m.%d %H:%M:%S"), platform, variant );
goal = goal or "";
jobs = jobs or 4;

build:initialize {
    variants = { 'debug', 'release', 'shipping' };
    bin = build:root( ("../%s/bin"):format(variant) );
    lib = build:root( ("../%s/lib"):format(variant) );
    obj = build:root( ("../%s/obj"):format(variant) );
    include_directories = {
        build:root()
    };
    library_directories = {
        build:root( ("../%s/lib"):format(variant) )
    };
    visual_studio = {
        sln = build:root( "../sweet_lalr.sln" );
    };
    xcode = {
        xcodeproj = build:root( "../sweet_lalr.xcodeproj" );
    };
};

build:default_targets {
    "sweet/lalr_examples",
    "sweet/lalr_test"
};

buildfile "sweet/lalr/lalr.build";
buildfile "sweet/lalr_examples/lalr_examples.build";
buildfile "sweet/lalr_test/lalr_test.build";
buildfile "unit/unit.build";
