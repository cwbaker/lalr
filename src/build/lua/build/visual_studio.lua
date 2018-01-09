
visual_studio = {};

-- Generate a UUID by calling the uuidgen tool.
local function uuid()    
    local uuids = {};
    local UuidScanner = Scanner {
        [ [[([A-Za-z0-9\-]+)]] ] = function( uuid )
            table.insert( uuids, uuid );
        end;
    };
    local uuidgen = "%s/Common7/Tools/uuidgen.exe" % settings.msvc.visual_studio_directory;
    local arguments = "uuidgen";
    build.system( uuidgen, arguments, UuidScanner );
    assert( uuids[1], "UUID generation failed!" );
    return upper( uuids[1] );
end

-- Generate the header of the vcproj.
local function header( vcproj, target )
    local name = target.project_name or target:id();
    vcproj:write( [[
<?xml version="1.0" encoding="Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="9.00"
	Name="]]..name..[["
	Keyword="MakeFileProj"
	>
	<Platforms>
        <Platform Name="Win32" />
	</Platforms>
]]
    );    
end

-- Generate the footer of the vcproj.
local function footer( vcproj )
    vcproj:write( [[
</VisualStudioProject>
]]
    );
end

-- Generate the configurations.
local function configurations( vcproj, target, include_directories )
    vcproj:write( [[
	<Configurations>
]]
    );

    pushd( target:directory() );
    local module = target:id();
    for _, platform in ipairs(target.settings.platforms) do
        for _, variant in pairs(target.settings.variants) do
            local variant_settings = target.settings.settings_by_variant[variant];
            
            local output = "";
            if target:prototype() == ExecutablePrototype then
                output = relative( target.settings.bin.."/"..module.."_"..platform.."_"..variant..".exe" );
            end
            
            local defines = "";
            defines = defines.."BUILD_PLATFORM_"..upper(platform)..";";
            defines = defines.."BUILD_VARIANT_"..upper(variant)..";";
            defines = defines.."BUILD_MODULE_"..upper(string.gsub(module, "-", "_"))..";"
            defines = defines.."BUILD_LIBRARY_TYPE_"..upper(variant_settings.library_type)..";";
            if target.settings.defines then
                for _, define in ipairs(target.settings.defines) do
                    defines = defines..define..";";
                end
            end    
            if target.defines then
                for _, define in ipairs(target.defines) do
                    defines = defines..define..";";
                end
            end

            local build_tool = native( relative(root("build/build.exe")) );
            vcproj:write( [[
            <Configuration
                Name="]]..platform..[[_]]..variant..[[|Win32"
                OutputDirectory="../../../../obj/]]..platform..[[_]]..variant..[[/]]..relative(target:directory(), root())..[["
                IntermediateDirectory="../../../../obj/]]..platform..[[_]]..variant..[[/]]..relative(target:directory(), root())..[["
                ConfigurationType="0"
                >
                <Tool
                    Name="VCNMakeTool"
                    BuildCommandLine="]]..build_tool..[[ variant=]]..variant..[[ platform=]]..platform..[[ goal=]]..target:id()..[["
                    ReBuildCommandLine="]]..build_tool..[[ clean default variant=]]..variant..[[ platform=]]..platform..[[ goal=]]..target:id()..[["
                    CleanCommandLine="]]..build_tool..[[ clean variant=]]..variant..[[ platform=]]..platform..[[ goal=]]..target:id()..[["
                    Output="]]..output..[["
                    PreprocessorDefinitions="]]..defines..[["
                    IncludeSearchPath="]]..include_directories..[["
                    ForcedIncludes=""
                    AssemblySearchPath=""
                    ForcedUsingAssemblies=""
                    CompileAsManaged=""
                />
            </Configuration>
    ]]
            );
        end    
    end
    popd();
    
    vcproj:write( [[
	</Configurations>
]]
    );
end

-- Write the beginning of the Files element.
local function begin_files( vcproj )
    vcproj:write( [[
	<Files>
]]
    );
end

-- Write the end of the Files element.
local function end_files( vcproj, module )
    vcproj:write( [[
        <File RelativePath=".\]]..leaf(module:directory())..[[.build" />
	</Files>
]]
    );
end

-- Generate files in the project.
local function generate_files( vcproj, directory, files, level )
    pushd( directory );
    for _, file in ipairs(files) do
        vcproj:write( string.rep("    ", level) );
        vcproj:write( "<File RelativePath=\""..native(relative(file:get_filename())).."\" />\n" );
    end
    popd();
end

-- Generate a Visual Studio project for /module/ to /filename/.
local function generate_project( filename, module )
    print( filename );

    local function populate_files( files, directory )
        return function( target )
            if target:directory() == directory and (target:prototype() == SourceFilePrototype or target:prototype() == HeaderFilePrototype) then
                table.insert( files, target );
            end
        end
    end

    local include_directories = "";
    for _, directory in ipairs(module.settings.include_directories) do
        include_directories = include_directories..native(directory)..";";
    end
    
    local files = {};
    preorder( populate_files(files, module:directory()), module );

    local vcproj = io.open( absolute(filename), "wb" );
    header( vcproj, module );
    configurations( vcproj, module, include_directories );
    begin_files( vcproj );
    generate_files( vcproj, module:directory(), files, 2 );
    end_files( vcproj, module );
    footer( vcproj );
    vcproj:close();    
end

-- Generate a Visual Studio solution for /solution/, containing /modules/ 
-- and /directories/ to /filename/.
local function generate_solution( filename, solution, modules, directories )
    print( filename );

    local sln = io.open( absolute(filename), "wb" );
    sln:write( [[
Microsoft Visual Studio Solution File, Format Version 10.00
# Visual Studio 2008
]]  
    );

    pushd( branch(absolute(filename)) );
    local project_uuid = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
    for path, module in pairs(modules) do
        local name = module.target.project_name or module.target:id();
        local filename = "%s%s.vcproj" % { module.target:directory(), name };
        sln:write( string.format([[
Project("{%s}") = "%s", "%s", "{%s}"
EndProject
]], project_uuid, name, native(relative(filename)), module.uuid)
        );
    end;

    local folder_uuid = "2150E333-8FDC-42A3-9474-1A3956D46DE8";
    local function generate_directories( directories )
        for _, directory in ipairs(directories) do
            if directory.children then
                sln:write([[
Project("{%s}") = "%s", "%s", "{%s}"
EndProject
]] 
                % {folder_uuid, directory.target:id(), directory.target:id(), directory.uuid}
                );
                generate_directories( directory.children );
            end
        end
    end
    generate_directories( directories );

    local build_hpp = native( relative(root("build.hpp")) );
    local build_lua = native( relative(root("build.lua")) );
    local local_settings_lua = native( relative(root("local_settings.lua")) );
    sln:write( [[
Project("{%s}") = "Solution Items", "Solution Items", "{%s}"
	ProjectSection(SolutionItems) = preProject
		%s = %s
		%s = %s
		%s = %s
	EndProjectSection
EndProject
]] % {folder_uuid, uuid(), build_hpp, build_hpp, build_lua, build_lua, local_settings_lua, local_settings_lua }
    );
    popd();

    sln:write([[
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
]]
    );
    for _, platform in ipairs(build.settings.platforms) do
        for _, variant in ipairs(build.settings.variants) do
            sln:write([[
		%s_%s|Win32 = %s_%s|Win32
]]
            % {platform, variant, platform, variant}
            );
        end
    end

    sln:write([[
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
]]
    );

    for _, module in pairs(modules) do
        for _, platform in ipairs(module.target.settings.platforms) do
            for _, variant in ipairs(module.target.settings.variants) do
                sln:write([[
		{%s}.%s_%s|Win32.ActiveCfg = %s_%s|Win32
		{%s}.%s_%s|Win32.Build.0 = %s_%s|Win32
]]
                % {module.uuid, platform, variant, platform, variant, module.uuid, platform, variant, platform, variant}
                );
            end
        end
    end

    sln:write([[
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	GlobalSection(NestedProjects) = preSolution
]]
    );

    local function generate_nested_projects( directory )
        if directory and directory.children then
            for _, child in ipairs(directory.children) do
                sln:write([[
		{%s} = {%s}
]] 
                % {child.uuid, directory.uuid}
                );
            end
            for _, directory in ipairs(directory.children) do
                generate_nested_projects( directory );
            end
        end
    end
    for _, directory in ipairs(directories) do
        generate_nested_projects( directory );
    end

    sln:write([[
	EndGlobalSection
EndGlobal
]]
    );

    sln:close();
end

-- Configure the Visual Studio module.
function visual_studio.configure()
end

-- Initiailize Visual Studio module.
function visual_studio.initialize()
end

-- Generate a Visual Studio solution and projects for the project and modules
-- that are dependencies of /root_target/ to /filename/.
function visual_studio.generate_solution_and_projects( filename, root_target )
    local function populate( modules, directories )
        local function add_directory( target )
            local directory = directories[target:path()];
            if directory == nil then
                directory = { target = target; children = {} };
                directories[target:path()] = directory;
                if target:parent() then
                    local parent = add_directory( target:parent() );
                    table.insert( parent.children, directory );
                end
            end
            return directory;
        end

        local function add_module( target )
            local module = modules[target:path()];
            if module == nil then
                module = { target = target };
                modules[target:path()] = module;
                if target:parent() then
                    local parent = add_directory( target:parent() );
                    table.insert( parent.children, module );
                end
            end
            return module;
        end

        return function( target )
            if target:prototype() == ExecutablePrototype or target:prototype() == StaticLibraryPrototype or target:prototype() == DynamicLibraryPrototype then
                add_module( target );
            end
        end
    end

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

    local function generate_uuids( objects )
        for _, object in pairs(objects) do
            object.uuid = uuid();
        end
    end

    local modules = {};
    local directories = {};
    preorder( populate(modules, directories), root_target );
    generate_uuids( modules );
    generate_uuids( directories );
    prune( directories[root_target:path()] );

    for path, module in pairs(modules) do
        local name = module.target.project_name or module.target:id();
        generate_project( "%s%s.vcproj" % {module.target:directory(), name}, module.target );
    end
    generate_solution( filename, root_target, modules, directories[root_target:path()].children );
end

-- The "sln" command entry point (global).
function sln()
    build.load();
    local all = all or find_target( root() );
    assert( all, "No target found at '"..tostring(root()).."'" );
    assert( settings.sln, "The solution filename is not specified by settings.sln" );
    visual_studio.generate_solution_and_projects( settings.sln, all );        
end
