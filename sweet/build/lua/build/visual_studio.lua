
--
-- Generate the header of the vcproj.
--
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

--
-- Generate the footer of the vcproj.
--
local function footer( vcproj )
    vcproj:write( [[
</VisualStudioProject>
]]
    );
end

--
-- Generate the configurations.
--
local function configurations( vcproj, target, include_directories )
    vcproj:write( [[
	<Configurations>
]]
    );

    local module = target:id();
    for variant, variant_settings in pairs(target.settings.variants) do
        local output = "";
        if target:rule() == Executable then
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

        vcproj:write( [[
        <Configuration
            Name="]]..variant..[[|Win32"
            OutputDirectory="../../../../obj/]]..platform..[[_]]..variant..[[/]]..relative(target:directory(), root())..[["
            IntermediateDirectory="../../../../obj/]]..platform..[[_]]..variant..[[/]]..relative(target:directory(), root())..[["
			ConfigurationType="0"
            >
            <Tool
                Name="VCNMakeTool"
                BuildCommandLine="build command=build variant=]]..variant..[[ platform=]]..platform..[[ target=]]..target:id()..[["
                ReBuildCommandLine="build command=clean variant=]]..variant..[[ platform=]]..platform..[[ target=]]..target:id()..[[ &#x0D;&#x0A; build command=build variant=]]..variant..[[ platform=]]..platform..[[ target=]]..target:id()..[["
                CleanCommandLine="build command=clean variant=]]..variant..[[ platform=]]..platform..[[ target=]]..target:id()..[["
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
    
    vcproj:write( [[
	</Configurations>
]]
    );
end

--
-- Write the beginning of the Files element.
--
local function begin_files( vcproj )
    vcproj:write( [[
	<Files>
]]
    );
end

--
-- Write the end of the Files element.
--
local function end_files( vcproj, module )
    vcproj:write( [[
        <File RelativePath=".\]]..leaf(module:directory())..[[.build" />
	</Files>
]]
    );
end

--
-- Generate files in the project that match \e include_patterns and not
-- \e exclude_patterns.
--
local function files( vcproj, directory, include_patterns, exclude_patterns, level )
    function match_( filename, patterns )    
        for _, pattern in ipairs(patterns) do
            if string.match(filename, pattern) then
                return true;
            end
        end
        return false;
    end

    vcproj:write( string.rep("    ", level) );
    vcproj:write( "<Filter Name=\""..basename(directory).."\" Filter=\"\">\n" );

    for entry in ls(directory) do
        if is_file(entry) and match_(entry, include_patterns) and not match_(entry, exclude_patterns) then
            vcproj:write( string.rep("    ", level + 1) );
            vcproj:write( "<File RelativePath=\""..native(relative(entry)).."\" />\n" );
       end    
    end

    vcproj:write( string.rep("    ", level) );
    vcproj:write( "</Filter>\n" );
end

--
-- Write out a Microsoft Visual Studio project for an Executable or Library.
--
function generate_visual_studio_project( filename, module )
    print( filename );

    local include_directories = "";
    for _, directory in ipairs(module.settings.include_directories) do
        include_directories = include_directories..native(directory)..";";
    end
    
    local vcproj = io.open( absolute(filename), "wb" );
    header( vcproj, module );
    configurations( vcproj, module, include_directories );
    begin_files( vcproj );
    local include_patterns = { ".*%.c$", ".*%.cpp$", ".*%.h$", ".*%.hpp$", ".*%.ipp$", ".*%.lua$", ".*%.g$" };
    local exclude_patterns = { "/moc_.*" };
    files( vcproj, absolute("."), include_patterns, exclude_patterns, 2 );
    end_files( vcproj, module );
    footer( vcproj );
    vcproj:close();    
end

function generate_visual_studio_solution( filename, solution )
    print( filename );

    local sln = io.open( absolute(filename), "wb" );
    sln:write( [[
Microsoft Visual Studio Solution File, Format Version 10.00
# Visual Studio 2008
]]  
    );

    for target in solution:get_dependencies() do
        if target:rule() == Project then
            sln:write( string.format([[
Project("{}") = "%s", "%s.vcproj", "{}"
EndProject
]], basename(target:get_id()), native(relative(target:path())))
            );
        end
    end;

    sln:write( [[
Project("{}") = "Solution Items", "Solution Items", "{}"
    ProjectSection(SolutionItems) = preProject
        build.hpp = build.hpp
        build.lua = build.lua
    EndProjectSection
EndProject
]]
    );

    sln:close();
end
