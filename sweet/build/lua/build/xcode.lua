
local id = 256;
local function uuid()
    id = id + 1;
    return "%08x%08x%08x" % { id, id, id };
end

local files = {};
local groups = {};
local targets = {};
local project_root = "";
local project_uuid = "";

local function add_group( target )
    assert( target );

    local path = target:path();
    local group = groups[path];
    if not group then
        group = { 
            uuid = uuid(); 
            target = target; 
            children = {}; 
        };
        groups[path] = group;

        if target:parent() and target:path() ~= root() then
            local parent = add_group( target:parent() );
            table.insert( parent.children, group );
        end
    end
    return group;
end

local function add_file( target )
    assert( target );

    if target:parent() then
        local path = target:path();
        local file = files[path];
        if not file then
            file = { 
                uuid = uuid(); 
                target = target 
            };
            files[path] = file;

            local group = add_group( target:parent() );
            table.insert( group.children, file );
        end
    end
    return file;
end

local function add_target( module )
    assert( module );

    local path = module:path();
    local target = targets[path];
    if not target then
        target = {
            uuid = uuid();
            target = module;
            configurations = {};
            configuration_list_uuid = uuid();
        };
        targets[path] = target;

        local buildfile = absolute("%s.build" % module:id(), module:get_working_directory():path());
        if exists(buildfile) then
            add_file( File(buildfile) );
        end

        for _, variant in pairs(module.settings.variants) do
            local configuration = {
                uuid = uuid();
                target = module;
                variant = variant;
            };
            table.insert( target.configurations, configuration );
        end
    end
    return target;
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
        xcodeproj:write([[
        %s /* %s */ = { isa = PBXFileReference; lastKnownFileType = text; path = %s; sourceTree = "<group>"; };
]]
        % { file.uuid, file.target:id(), file.target:id() }
        );
    end
end

local function generate_groups( xcodeproj, groups )

    local sorted_groups = {};
    for path, group in pairs(groups) do
        table.insert( sorted_groups, group );
    end
    table.sort( sorted_groups, function(lhs, rhs) return lhs.target:id() < rhs.target:id() end );

    for _, group in pairs(sorted_groups) do
        xcodeproj:write([[
        %s /* %s */ = {
            isa = PBXGroup;
            children = (
]]
        % { group.uuid, group.target:path() }
        );

        table.sort( group.children, function(lhs, rhs) 
            local lhs_file = 0;
            if is_file(lhs.target:path()) then
                lhs_file = 1;
            end
            local rhs_file = 0;
            if is_file(rhs.target:path()) then
                rhs_file = 1;
            end
            return 
                lhs_file < rhs_file or
                (lhs_file == rhs_file and lhs.target:id() < rhs.target:id())
            ;
        end);
        for _, child in ipairs(group.children) do
            xcodeproj:write([[
                %s /* %s */,
]]
            % { child.uuid, child.target:id() }
            );
        end;
        local base;
        if group.target:parent() then
            base = group.target:parent():path();
        else
            base = project_root;
        end
        xcodeproj:write([[
            );
            name = %s;
            path = %s;
            sourceTree = "<group>";
        };
]] % { group.target:id(), relative(group.target:path(), base) }
        );
    end
end

local function generate_targets( xcodeproj, targets )

    local sorted_targets = {};
    for path, target in pairs(targets) do
        table.insert( sorted_targets, target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.target:id() < rhs.target:id() end );

    for _, target in ipairs(sorted_targets) do 
        local name = target.target:id();
        xcodeproj:write([[
        %s /* %s */ = {
            isa = PBXLegacyTarget;
            buildArgumentsString = "variant=$(CONFIGURATION) action=$(ACTION) xcode_build";
            buildConfigurationList = %s /* Build configuration list for PBXLegacyTarget "%s" */;
            buildPhases = (
            );
            buildToolPath = %s;
            buildWorkingDirectory = %s;
            dependencies = (
            );
            name = %s;
            passBuildSettingsInEnvironment = 1;
            productName = %s;
        };
]]
        % { target.uuid, name, target.configuration_list_uuid, name, home("bin/build"), target.target:get_working_directory():path(), name, name }
        );  
    end
end

local function generate_project( xcodeproj, groups )
    local main_group = groups[root()];
    assert( main_group, "The main group for the Xcode project wasn't found" );

    project_uuid = uuid();

    xcodeproj:write([[
    %s /* Project object */ = {
        isa = PBXProject;
        attributes = {
            LastUpgradeCheck = 0430;
        };
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
]]
    % { project_uuid, main_group.uuid }
    );

    local sorted_targets = {};
    for path, target in pairs(targets) do
        table.insert( sorted_targets, target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.target:id() < rhs.target:id() end );

    for _, target in ipairs(sorted_targets) do
        xcodeproj:write([[
            %s /* %s */,
]]
        % { target.uuid, target.target:id() }
        );
    end
    xcodeproj:write([[
        );
    };
]]
    );
end

local function generate_configurations( xcodeproj, targets )
    local sorted_targets = {};
    for path, target in pairs(targets) do
        table.insert( sorted_targets, target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.target:id() < rhs.target:id() end );

    for _, target in pairs(sorted_targets) do 
        table.sort( target.configurations, function(lhs, rhs) return lhs.variant < rhs.variant end );
        for _, configuration in ipairs(target.configurations) do
            local archs = table.concat( target.target.settings.architectures or {}, " " );
            xcodeproj:write([[
    %s /* %s */ = {
        isa = XCBuildConfiguration;
        buildSettings = {
            ARCHS = "%s";
            VALID_ARCHS = "%s";
        };
        name = %s;
    };
]]
            % { configuration.uuid, target.target:id(), archs, archs, configuration.variant }
            );
        end
    end
end

local function generate_configuration_lists( xcodeproj, targets )
    local sorted_targets = {};
    for path, target in pairs(targets) do
        table.insert( sorted_targets, target );
    end
    table.sort( sorted_targets, function(lhs, rhs) return lhs.target:id() < rhs.target:id() end );

    for _, target in pairs(sorted_targets) do
        xcodeproj:write([[
    %s /* Build configuration list for PBXLegacyTarget "%s" */ = {
        isa = XCConfigurationList;
        buildConfigurations = (
]]
        % { target.configuration_list_uuid, target.target:id() }
        );
        for _, configuration in ipairs(target.configurations) do
            xcodeproj:write([[
            %s /* %s */,
]]
            % { configuration.uuid, configuration.variant }
            );
        end        
        xcodeproj:write([[
        );
        defaultConfigurationIsVisible = 0;
    };
]]
        );
    end
end


local function footer( xcodeproj, project )
    xcodeproj:write([[
    };
    rootObject = %s /* Project object */;
}
]]
    % project_uuid
    );
end;

local function populate( target )
    if target:prototype() == SourceFilePrototype or target:prototype() == HeaderFilePrototype then
        add_file( target );
    elseif target:prototype() == Directory then
        add_group( target );
    elseif target:prototype() == ExecutablePrototype then
        add_target( target );
    end
end

xcode = {};

function xcode.configure( settings )
end

function xcode.initialize( settings )
    xcode.configure( settings );
end

function xcode.generate_project( name, project )
    project_root = branch( name );
    preorder( populate, project );
    if exists(root("build.lua")) then
        add_file( find_target(root("build.lua")) );
    end

    local xcodeproj_directory = "%s" % name;
    if not exists(xcodeproj_directory) then
        mkdir( xcodeproj_directory );
    end
    assert( is_directory(xcodeproj_directory), "The file '%s' already exists but is not a directory as expected" % xcodeproj_directory );

    local xcodeproj = io.open( absolute("project.pbxproj", xcodeproj_directory), "wb" );
    assert( xcodeproj, "Opening '%s' to write Xcode project file failed" % filename );
    header( xcodeproj, project );
    generate_files( xcodeproj, files );
    generate_groups( xcodeproj, groups );
    generate_targets( xcodeproj, targets );
    generate_project( xcodeproj, groups );
    generate_configurations( xcodeproj, targets );
    generate_configuration_lists( xcodeproj, targets );
    footer( xcodeproj, project );
    xcodeproj:close();
end

-- The "xcodeproj" command entry point (global).
function xcodeproj()
    build.load();
    local all = all or find_target( root() );
    assert( all, "No target found at '%s'" % root() );
    assert( settings.xcodeproj, "The project filename is not specified by settings.xcodeproj" );
    xcode.generate_project( settings.xcodeproj, all );
end

-- The "xcode_build" command entry point (global) this is used by generated
-- Xcode projects to trigger a build or clean.
function xcode_build()
    action = action or "build";
    if action == "" or action == "build" then
        default();
    elseif action == "clean" then
        clean();
    else
        error( "Unable to map the Xcode action '%s' to a build command" % tostring(action) );
    end
end
