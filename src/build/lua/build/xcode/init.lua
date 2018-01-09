
local id = 256;
local function uuid()
    id = id + 1;
    return ("%08x%08x%08x"):format( id, id, id );
end

local files = {};
local groups = {};
local legacy_targets = {};
local project_root = "";
local project_id = "";
local project_uuid = "";
local project_configuration_list_uuid = "";
local project_configurations = {};

local function add_group( path )
    local group = groups[path];
    if not group then
        group = { 
            uuid = uuid(); 
            path = path; 
            children = {};
        };
        groups[path] = group;

        local directory = build:branch( path );
        if directory ~= "" and not string.find(build:relative(directory, build:root()), "..", 1, true) then
            local parent = add_group( directory );
            table.insert( parent.children, group );
            group.parent = parent;
        end
    end
    return group;
end

local function add_file( filename )
    local directory = build:branch( filename );
    if directory ~= "" and not string.find(build:relative(filename, build:root()), "..", 1, true) then
        local file = files[path];
        if not file then
            file = { 
                uuid = uuid(); 
                path = filename 
            };
            files[filename] = file;

            local group = add_group( directory );
            table.insert( group.children, file );
            file.parent = group;
        end
    end
    return file;
end

local function add_configurations( architectures, variants )
    local configurations = {};
    for _, variant in pairs(variants) do
        local configuration = {
            uuid = uuid();
            architectures = architectures;
            variant = variant;
        };
        table.insert( configurations, configuration );
    end
    return configurations;
end

local function add_legacy_target( target, platform, architectures )
    assert( target );
    local filename = target:filename();
    local working_directory = target:working_directory():path();
    local settings = target.settings;
    local legacy_target = legacy_targets [filename];
    if not legacy_target then
        legacy_target = {
            uuid = uuid();
            name = build:leaf( filename );
            working_directory = working_directory;
            configuration_list = uuid();
            platform = platform;
            build = build:executable( "build" );
            path = filename;
            settings = settings;
            configurations = add_configurations( architectures, settings.variants );
        };
        legacy_targets[filename] = legacy_target;
    end
    return legacy_target;
end

local function header( xcodeproj, project )
    xcodeproj:write([[
// !$*UTF8*$!
{
    archiveVersion = 1;
    classes = {
    };
    objectVersion = 46;
    objects = {
]]
    );
end;

local function generate_files( xcodeproj, files )
    for path, file in pairs(files) do
        local filename = build:leaf( file.path );
        xcodeproj:write(([[
        %s /* %s */ = { isa = PBXFileReference; lastKnownFileType = text; path = "%s"; sourceTree = "<group>"; };
]]):format(file.uuid, filename, filename)
        );
    end
end

local function generate_groups( xcodeproj, groups )
    local sorted_groups = {};
    for path, group in pairs(groups) do
        table.insert( sorted_groups, group );
    end
    table.sort( sorted_groups, function(lhs, rhs) return lhs.path < rhs.path end );

    for _, group in pairs(sorted_groups) do
        xcodeproj:write(([[
        %s /* %s */ = {
            isa = PBXGroup;
            children = (
]]):format(group.uuid, group.path)
        );

        table.sort( group.children, function(lhs, rhs) 
            local lhs_file = 0;
            if build:is_file(lhs.path) then
                lhs_file = 1;
            end
            local rhs_file = 0;
            if build:is_file(rhs.path) then
                rhs_file = 1;
            end
            return 
                lhs_file < rhs_file or
                (lhs_file == rhs_file and lhs.path < rhs.path)
            ;
        end);
        for _, child in ipairs(group.children) do
            xcodeproj:write(([[
                %s /* %s */,
]]):format(child.uuid, child.path)
            );
        end;
        local base;
        if group.parent then
            base = group.parent.path;
        else
            base = project_root;
        end
        xcodeproj:write(([[
            );
            name = "%s";
            path = "%s";
            sourceTree = "<group>";
        };
]]):format(build:leaf(group.path), build:relative(group.path, base))
        );
    end
end

local function generate_legacy_targets( xcodeproj, legacy_targets )
    local sorted_targets = {};
    for path, target in pairs(legacy_targets) do
        table.insert( sorted_targets, target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.path < rhs.path end );

    for _, legacy_target in ipairs(sorted_targets) do 
        local name = build:leaf( legacy_target.path );
        local template = [[
        ${uuid} /* ${name} */ = {
            isa = PBXLegacyTarget;
            buildArgumentsString = "platform=${platform} variant=$(CONFIGURATION) action=$(ACTION) xcode_build";
            buildConfigurationList = ${configuration_list} /* Build configuration list for PBXLegacyTarget "${name}" */;
            buildPhases = (
            );
            buildToolPath = "${build}";
            buildWorkingDirectory = "${working_directory}";
            dependencies = (
            );
            name = "${name}";
            passBuildSettingsInEnvironment = 1;
            productName = "${name}";
        };
]];
        xcodeproj:write( build:interpolate(template, legacy_target) );
    end
end

local function generate_project( xcodeproj, groups )
    local main_group = groups[build:root()];
    assert( main_group, "The main group for the Xcode project wasn't found" );

    project_uuid = uuid();

    xcodeproj:write(([[
    %s /* Project object */ = {
        isa = PBXProject;
        attributes = {
            LastUpgradeCheck = 0430;
        };
        buildConfigurationList = %s /* Build configuration list for PBXProject "%s" */;
        compatibilityVersion = "Xcode 3.2";
        developmentRegion = English;
        hasScannedForEncodings = 0;
        knownRegions = (
            en,
        );
        mainGroup = %s;
        projectDirPath = "";
        projectRoot = "";
        targets = (
]]):format(project_uuid, project_configuration_list_uuid, project_id, main_group.uuid)
    );

    local sorted_targets = {};
    for path, legacy_target in pairs(legacy_targets) do
        table.insert( sorted_targets, legacy_target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.path < rhs.path end );

    for _, target in ipairs(sorted_targets) do
        xcodeproj:write(([[
            %s /* %s */,
]]):format(target.uuid, build:leaf(target.path))
        );
    end
    xcodeproj:write([[
        );
    };
]]
    );
end

local function generate_configuration( xcodeproj, configuration, id, settings )
    local archs = table.concat( configuration.architectures or {settings.architecture} or {}, " " );
    xcodeproj:write(([[
    %s /* %s */ = {
        isa = XCBuildConfiguration;
        buildSettings = {
            ARCHS = "%s";
            VALID_ARCHS = "%s";
            ONLY_ACTIVE_ARCH = YES;
]]):format(configuration.uuid, id, archs, archs)
    );

    if settings.sdkroot then 
        xcodeproj:write(([[
            SDKROOT = "%s";
]]):format(settings.sdkroot)
        );
    end

    if settings.iphoneos_deployment_target then 
        xcodeproj:write(([[
            IPHONEOS_DEPLOYMENT_TARGET = "%s";
]]):format(settings.iphoneos_deployment_target)
        );
    end

    if settings.targeted_device_family then 
        xcodeproj:write(([[
            TARGETED_DEVICE_FAMILY = "%s";
]]):format(settings.targeted_device_family)
        );
    end

    xcodeproj:write(([[ 
        };
        name = %s;
    };
]]):format(configuration.variant)
    );
end

local function generate_configurations( xcodeproj, legacy_targets )
    table.sort( project_configurations, function(lhs, rhs) return lhs.variant < rhs.variant end );
    for _, configuration in ipairs(project_configurations) do
        generate_configuration( xcodeproj, configuration, project_id, build.settings );
    end

    local sorted_targets = {};
    for path, legacy_target in pairs(legacy_targets) do
        table.insert( sorted_targets, legacy_target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.path < rhs.path end );

    for _, target in pairs(sorted_targets) do 
        table.sort( target.configurations, function(lhs, rhs) return lhs.variant < rhs.variant end );
        for _, configuration in ipairs(target.configurations) do
            generate_configuration( xcodeproj, configuration, target.path, target.settings );
        end
    end
end

local function generate_configuration_list( xcodeproj, uuid, id, configurations )
        xcodeproj:write(([[
    %s /* Build configuration list for PBXLegacyTarget "%s" */ = {
        isa = XCConfigurationList;
        buildConfigurations = (
]]):format(uuid, id)
        );
        for _, configuration in ipairs(configurations) do
            xcodeproj:write(([[
            %s /* %s */,
]]):format(configuration.uuid, configuration.variant)
            );
        end        
        xcodeproj:write([[
        );
        defaultConfigurationIsVisible = 0;
    };
]]
        );
end

local function generate_configuration_lists( xcodeproj, legacy_targets )
    local sorted_targets = {};
    for path, target in pairs(legacy_targets) do
        table.insert( sorted_targets, target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.path < rhs.path end );

    generate_configuration_list( xcodeproj, project_configuration_list_uuid, project_id, project_configurations );
    for _, target in pairs(sorted_targets) do
        generate_configuration_list( xcodeproj, target.configuration_list, build:leaf(target.path), target.configurations );
    end
end

local function generate_build_phases( xcodeproj, build_phases )
    for _, build_phase in ipairs(build_phases) do
        local template = [[
    ${uuid} /* ShellScript */ = {
        isa = PBXShellScriptBuildPhase;
        buildActionMask = 2147483647;
        files = (
        );
        inputPaths = (
        );
        outputPaths = (
        );
        runOnlyForDeploymentPostprocessing = 0;
        shellPath = /bin/sh;
        shellScript = "${command_line}";
    };
]];
    xcodeproj:write( build:interpolate(template, build_phase) );
    end
end

local function footer( xcodeproj, project )
    xcodeproj:write(([[
    };
    rootObject = %s /* Project object */;
}
]]):format(project_uuid)
    );
end;

local function find_targets_by_prototype( target, prototype )
    local targets = {};
    if target:prototype() == prototype then 
        table.insert( targets, target );
        return targets;
    end
    for _, dependency in target:dependencies() do 
        if dependency:prototype() == prototype then 
            table.insert( targets, dependency );
        end
    end
    return targets;
end

local function find_architectures_by_prototype( target, prototype, architectures )
    local architectures = architectures or {};
    if target:prototype() == prototype then 
        local architecture = target.settings.architecture;
        table.insert( architectures, architecture );
    else
        for _, dependency in target:dependencies() do 
            find_architectures_by_prototype( dependency, prototype, architectures );
        end
    end
    return architectures;
end

local function included( filename, includes, excludes )
    if build:is_directory(filename) then 
        return false;
    end

    if excludes then 
        for _, pattern in ipairs(excludes) do 
            if string.match(filename, pattern) then 
                return false;
            end
        end
    end

    if includes then 
        for _, pattern in ipairs(includes) do 
            if string.match(filename, pattern) then 
                return true;
            end
        end
        return false;
    end

    return true;
end

local function populate_source( source, includes, excludes )
    for filename in build:find( source ) do 
        if included(filename, includes, excludes) then 
            add_file( filename );
        end
    end
end

xcode = {};

function xcode.configure( settings )
end

function xcode.initialize( settings )
    xcode.configure( settings );
end

function xcode.generate_project( name, project )
    project_root = build:branch( name );
    project_id = build:leaf( build:basename(name) );
    project_configuration_list_uuid = uuid();
    project_configurations = add_configurations( nil, build.settings.variants );

    populate_source( build:root(), {"^.*%.cp?p?$", "^.*%.hp?p?$", "^.*%.mm?$", "^.*%.java"}, {"^.*%.framework"} );

    for _, target in project:dependencies() do 
        if target then 
            if _G.ios then
                local ios_apps = find_targets_by_prototype( target, build.ios.App );
                for _, ios_app in ipairs(ios_apps) do 
                    local architectures = find_architectures_by_prototype( target, build.Executable );
                    add_legacy_target( ios_app, platform, architectures );
                end
            end

            if _G.android then
                local android_apks = find_targets_by_prototype( target, build.android.Apk );
                for _, android_apk in ipairs(android_apks) do 
                    add_legacy_target( android_apk, platform );
                end
            end

            if _G.macosx or _G.windows then
                local executables = find_targets_by_prototype( target, build.Executable );
                for _, executable in ipairs(executables) do 
                    add_legacy_target( executable, platform );
                end

                local dynamic_libraries = find_targets_by_prototype( target, build.DynamicLibrary );
                for _, dynamic_library in ipairs(dynamic_libraries) do 
                    add_legacy_target( dynamic_library, platform );
                end

                local binaries = find_targets_by_prototype( target, build.xcode.Lipo );
                for _, binary in ipairs(binaries) do 
                    add_legacy_target( binary, platform );
                end
            end
        end
    end

    local xcodeproj_directory = name;
    if build:exists( xcodeproj_directory ) then
        build:rmdir( xcodeproj_directory );
    end
    build:mkdir( xcodeproj_directory );

    local filename = build:absolute( "project.pbxproj", xcodeproj_directory );
    local xcodeproj = io.open( filename, "wb" );
    assertf( xcodeproj, "Opening '%s' to write Xcode project file failed", filename );
    header( xcodeproj, project );
    generate_files( xcodeproj, files );
    generate_groups( xcodeproj, groups );
    generate_legacy_targets( xcodeproj, legacy_targets );
    generate_project( xcodeproj, groups );
    generate_configurations( xcodeproj, legacy_targets );
    generate_configuration_lists( xcodeproj, legacy_targets );
    footer( xcodeproj, project );
    xcodeproj:close();
end

-- The "xcodeproj" command entry point (global).
function xcodeproj()
    local all = all or build:find_target( build:root("all") );
    assertf( all, "Missing target at '%s' to generate Xcode project from", build:root() );
    assertf( build.settings.xcode, "Missing Xcode settings in 'settings.xcode'" );
    assertf( build.settings.xcode.xcodeproj, "Missing Xcode project filename in 'settings.xcode.xcodeproj'" );
    xcode.generate_project( settings.xcode.xcodeproj, all );
end

-- The "xcode_build" command entry point (global) this is used by generated
-- Xcode projects to trigger a build or clean.
function xcode_build()
    build:set_stack_trace_enabled( true );    
    action = action or "build";
    if action == "" or action == "build" then
        local failures = default();
        assertf( failures == 0, "%d failures", failures );
        if failures == 0 then 
            if platform == "ios" then
                ios.deploy( build:find_target(build:initial("all")) );
            elseif platform == "android" then
                android.deploy( build:find_target(build:initial("all")) );
            end
        end
    elseif action == "clean" then
        clean();
    else
        error( ("Unable to map the Xcode action '%s' to a build command"):format(tostring(action)) );
    end
end

require "build.xcode.Plist";
require "build.xcode.Lipo";
require "build.xcode.Xib";

build:register_module( xcode );
