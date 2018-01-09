
local visual_studio = {};

require "build.visual_studio.vcxproj";
require "build.visual_studio.sln";

-- Generate a UUID by calling the uuidgen tool.
local function uuid()    
    local uuids = {};
    local uuidgen = ("%s/bin/x64/uuidgen.exe"):format( settings.msvc.windows_sdk_directory );
    local arguments = "uuidgen";
    build:system( uuidgen, arguments, nil, nil, function(line)
        local uuid = line:match( "[%w-_]+" );
        if uuid then 
            table.insert( uuids, uuid );
        end
    end );
    assert( uuids[1], "UUID generation failed!" );
    return upper( uuids[1] );
end

-- Recursively add a directory and its parents to the hierarchy to display in
-- the solution.
local function add_directory( directories, target )
    if target then 
        local directory = directories [target:path()];
        if directory == nil then
            directory = { target = target; children = {} };
            directories[target:path()] = directory;
            if target:parent() then
                local parent = add_directory( directories, target:parent() );
                if parent then 
                    table.insert( parent.children, directory );
                end
            end
        end
        return directory;
    end
end

-- Add a target to the projects to generate and recursively add its parent
-- directories to the hierarchy to display in the solution.
local function add_project( projects, directories, target )
    local project = projects[target:path()];
    if project == nil then
        project = { target = target };
        projects[target:path()] = project;
        if target:parent() then
            local parent = add_directory( directories, target:parent() );
            table.insert( parent.children, project );
        end
    end
    return project;
end

-- Find `Executable`, `StaticLibrary`, and `DynamicLibrary` targets to 
-- generate projects and a directory hierarchy for.
local function find_projects( target, projects, directories )
	local prototype = target:prototype();
	if prototype == build.Executable or prototype == build.StaticLibrary or prototype == build.DynamicLibrary then 
        add_project( projects, directories, target );
	end
	for _, dependency in target:dependencies() do 
		find_projects( dependency, projects, directories );
	end
end

-- Prune directories with only a single child by replacing them with their
-- single child.
local function prune( directory )
    if directory and directory.children then
        if #directory.children == 1 then
            local pruned_child = directory.children[1];
            directory.uuid = pruned_child.uuid;
            directory.target = pruned_child.target;
            directory.children = pruned_child.children;
        else
            for _, child in ipairs(directory.children) do 
                prune( child );
            end
        end
    end
end

-- Generate UUIDs to identify projects and directories.
local function generate_uuids( objects )
    for _, object in pairs(objects) do
        object.uuid = uuid();
    end
end

local function filter( filename, includes, excludes )
    if is_directory(filename) then 
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

local function ls( path, includes, excludes )
    local files = {};
    for filename in build:ls(path or pwd()) do 
        if filter(filename, includes, excludes) then
            table.insert( files, filename );
        end
    end
    return files;
end

-- Generate a Visual Studio solution with projects for all of the 
-- `Executable`, `StaticLibrary`, and `DynamicLibrary` targets that are 
-- recursively dependencies of the root directory.
function visual_studio.solution()
    platform = "";
    variant = "";
    build:load();    
    local all = build:find_target( build:root() );
    assertf( all, "Missing target at '%s' to generate Visual Studio solution from", build:root() );
    assertf( build.settings.visual_studio, "Missing Visual Studio settings in 'settings.visual_studio'" );
    assertf( build.settings.visual_studio.sln, "Missing solution filename in 'settings.visual_studio.sln'" );

    local directories = {};
    local projects = {};
    find_projects( all, projects, directories );
    generate_uuids( projects );
    generate_uuids( directories );
    prune( directories[all:path()] );

    for _, project in pairs(projects) do 
        local target = project.target;
        target.uuid = project.uuid;
    	build:pushd( target:working_directory():path() );
		local DEFAULT_SOURCE = { "^.*%.cp?p?$", "^.*%.hp?p?$", "^.*%.mm?$", "^.*%.java$" };
		local files = build:ls( build:pwd(), DEFAULT_SOURCE );
		build.visual_studio.vcxproj.generate( target, files );
		build:popd();
    end

    build.visual_studio.sln.generate( build.settings.visual_studio.sln, projects, directories[all:path()].children );
end

_G.sln = solution;

return visual_studio;
