
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

local boost_include_directory, boost_library_directory;
if build:operating_system() == 'windows' then
    boost_include_directory = "C:/boost/include/boost-1_62";
    boost_library_directory = "C:/boost/lib";
elseif build:operating_system() == 'macos' then
    boost_include_directory = "/usr/local/include";
    boost_library_directory = "/usr/local/lib";
end

build:initialize {
    variants = { 'debug', 'release', 'shipping' };
    bin = build:root( ("../%s/bin"):format(variant) );
    lib = build:root( ("../%s/lib"):format(variant) );
    obj = build:root( ("../%s/obj"):format(variant) );
    include_directories = {
        build:root(),
        boost_include_directory
    };
    library_directories = {
        build:root( ("../%s/lib"):format(variant) ),
        boost_library_directory
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
    "sweet/parser/parser_test",
    "sweet/parser/parser_"
};

buildfile "lua/lua.build";
buildfile "sweet/assert/assert.build";
buildfile "sweet/atomic/atomic.build";
buildfile "sweet/cmdline/cmdline.build";
buildfile "sweet/error/error.build";
buildfile "sweet/lexer/lexer.build";
buildfile "sweet/lua/lua.build";
buildfile "sweet/parser/parser.build";
buildfile "sweet/pointer/pointer.build";
buildfile "sweet/rtti/rtti.build";
buildfile "sweet/traits/traits.build";
buildfile "sweet/utility/utility.build";
buildfile "unit/unit.build";
