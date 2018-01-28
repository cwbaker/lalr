
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
        sln = build:root( "../sweet_parser.sln" );
    };
    xcode = {
        xcodeproj = build:root( "../sweet_parser.xcodeproj" );
    };
};

build:default_targets {
    "sweet/lexer/lexer_test",    
    "sweet/parser/parser_examples",
    "sweet/parser/parser_test"
};

buildfile "sweet/assert/assert.build";
buildfile "sweet/lexer/lexer.build";
buildfile "sweet/parser/parser.build";
buildfile "unit/unit.build";
