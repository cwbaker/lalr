
local sln = {};

local function generate_header( write )
    local HEADER = [[
Microsoft Visual Studio Solution File, Format Version 12.00
# Visual Studio 2013
VisualStudioVersion = 12.0.31101.0
MinimumVisualStudioVersion = 10.0.40219.1
]];
	write( HEADER );
end

local function generate_projects( write, projects )
    local PROJECT = [[
Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "%s", "%s", "{%s}"
EndProject
]];
    for _, project in pairs(projects) do
        local target = project.target;
        local name = target.project_name or target:id();
        local filename = ("%s.vcxproj"):format( target:path(), name );
        write( PROJECT, name, native(relative(filename)), project.uuid );
    end
end

local function generate_directories( write, directories )
	local DIRECTORY = [[
Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "%s", "%s", "{%s}"
EndProject
]];

    for _, directory in pairs(directories) do
        if directory.children then
        	write( DIRECTORY, directory.target:id(), directory.target:id(), directory.uuid );
            generate_directories( write, directory.children );
        end
    end
end

local function generate_files( write, files )
	--[=[
	local START_FILES = [[
Project("{%s}") = "Solution Items", "Solution Items", "{%s}"
	ProjectSection(SolutionItems) = preProject
]];
	local FILE = [[
		%s = %s
]];
	local FINISH_FILES = [[
	EndProjectSection
EndProject
]];
	write( START_FILES );
	for _, file in ipairs(files) do 
		write( FILE, file, file );
	end
	write( FINISH_FILES );
	]=]
end

local function generate_global( write )
	local GLOBAL = [[
Global
]];
	write( GLOBAL );
end

local function generate_solution_configuration_platforms( write )
	local START_SOLUTION_CONFIGURATION_PLATFORMS = [[
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
]];
	local SOLUTION_CONFIGURATION_PLATFORM = [[
		%s_%s|Win32 = %s_%s|Win32
]];
	local FINISH_SOLUTION_CONFIGURATION_PLATFORMS = [[
	EndGlobalSection
]];

    write( START_SOLUTION_CONFIGURATION_PLATFORMS );
    for _, platform in ipairs(build.settings.platforms) do
        for _, variant in ipairs(build.settings.variants) do
            write( SOLUTION_CONFIGURATION_PLATFORM, platform, variant, platform, variant );
        end
    end
    write( FINISH_SOLUTION_CONFIGURATION_PLATFORMS );
end

local function generate_project_configuration_platforms( write, targets )
	local START_PROJECT_CONFIGURATION_PLATFORMS = [[
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
]];
	local PROJECT_CONFIGURATION_PLATFORM = [[
		{%s}.%s_%s|Win32.ActiveCfg = %s_%s|Win32
		{%s}.%s_%s|Win32.Build.0 = %s_%s|Win32
]];
	local FINISH_PROJECT_CONFIGURATION_PLATFORMS = [[
	EndGlobalSection
]];

    local platforms = { "windows" };
    local variants = { "debug", "release", "shipping" };
    write( START_PROJECT_CONFIGURATION_PLATFORMS );
    for _, target in pairs(targets) do
        for _, platform in ipairs(platforms) do
            for _, variant in ipairs(variants) do
            	write( PROJECT_CONFIGURATION_PLATFORM, 
            		target.uuid, 
            		platform, variant, 
            		platform, variant, 
            		target.uuid, 
            		platform, variant, 
            		platform, variant
                );
            end
        end
    end
    write( FINISH_PROJECT_CONFIGURATION_PLATFORMS );
end

local function generate_solution_properties( write )
	local SOLUTION_PROPERTIES = [[
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
]];
	write( SOLUTION_PROPERTIES );
end

local function generate_nested_projects( write, directories )
	local START_NESTED_PROJECTS = [[
	GlobalSection(NestedProjects) = preSolution
]];

	local NESTED_PROJECT = [[
		{%s} = {%s}
]];

    local FINISH_NESTED_PROJECTS = [[
	EndGlobalSection
]];

    local function nested_projects( directory )
        if directory and directory.children then
            for _, child in ipairs(directory.children) do
                write( NESTED_PROJECT, child.uuid, directory.uuid );
            end
            for _, directory in ipairs(directory.children) do
                nested_projects( directory );
            end
        end
    end

	write( START_NESTED_PROJECTS );
    for _, directory in pairs(directories) do
        nested_projects( directory );
    end
	write( FINISH_NESTED_PROJECTS );
end

local function generate_end_global( write )
	local END_GLOBAL = [[
EndGlobal
]];
	write( END_GLOBAL );
end

local function write_function( file )
	return function( format, ... )
		file:write( format:format(...) );
	end
end

function sln.generate( filename, projects, directories )
    print( leaf(filename) );
    pushd( branch(absolute(filename)) );
    local file = io.open( absolute(filename), "wb" );
    assertf( file, "Opening '%s' to write solution failed", absolute(filename) );
    local write = write_function( file );
    generate_header( write );
    generate_projects( write, projects );
    generate_directories( write, directories );
    generate_global( write );
    generate_files( write, {} );
    generate_solution_configuration_platforms( write, projects );
    generate_project_configuration_platforms( write, projects );
    generate_solution_properties( write );
    generate_nested_projects( write, directories );
    generate_end_global( write );
    file:close();
    file = nil;
end

return sln;
