
local vcxproj = {};

local function generate_header( target, write )
    local HEADER = [[
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="12.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
]];
    write( HEADER );
end

local function generate_project_configurations( target, write )
    local START_PROJECT_CONFIGURATIONS = [[
  <ItemGroup Label="ProjectConfigurations">
]];

    local PROJECT_CONFIGURATION = [[
    <ProjectConfiguration Include="%s_%s|Win32">
      <Configuration>%s_%s</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
]];

    local FINISH_PROJECT_CONFIGURATIONS = [[
  </ItemGroup>
]];

    write( START_PROJECT_CONFIGURATIONS );
    for _, platform in ipairs(target.settings.platforms) do
        for _, variant in ipairs(target.settings.variants) do
            write( PROJECT_CONFIGURATION, platform, variant, platform, variant );
        end
    end
    write( FINISH_PROJECT_CONFIGURATIONS );
end

local function generate_globals_property_group( target, write )
    local GLOBALS = [[
  <PropertyGroup Label="Globals">
    <Keyword>MakeFileProj</Keyword>
    <ProjectGuid>{%s}</ProjectGuid>
  </PropertyGroup>
]];
    write( GLOBALS, target.uuid );
end

local function generate_configuration_property_groups( target, write )
  local PROPERTY_GROUP = [[
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='%s_%s|Win32'" Label="Configuration">
    <ConfigurationType>Makefile</ConfigurationType>
    <PlatformToolset>v120</PlatformToolset>
  </PropertyGroup>
]];
    for _, platform in ipairs(target.settings.platforms) do 
        for _, variant in ipairs(target.settings.variants) do 
            write( PROPERTY_GROUP, platform, variant );
        end
    end
end

local function generate_import( target, write )
    local IMPORT = [[
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
]];
    write( IMPORT );
end

local function generate_project_file_version_property_group( target, write )
    local PROJECT_FILE_VERSION_PROPERTY_GROUP = [[
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <PropertyGroup>
    <_ProjectFileVersion>12.0.30501.0</_ProjectFileVersion>
  </PropertyGroup>
]];
    write( PROJECT_FILE_VERSION_PROPERTY_GROUP );
end

local function generate_property_groups( target, write )
    local PROPERTY_GROUP = [[
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='%s_%s|Win32'">
    <OutDir>%s</OutDir>
    <IntDir>%s</IntDir>
    <NMakeBuildCommandLine>%s</NMakeBuildCommandLine>
    <NMakeReBuildCommandLine>%s clean default</NMakeReBuildCommandLine>
    <NMakeCleanCommandLine>%s clean</NMakeCleanCommandLine>
    <NMakeOutput>%s</NMakeOutput>
    <NMakePreprocessorDefinitions>%s</NMakePreprocessorDefinitions>
    <NMakeIncludeSearchPath>%s</NMakeIncludeSearchPath>
  </PropertyGroup>
]];
    local build_tool = native( relative(root("build/build.exe")) );
    for _, platform in ipairs(target.settings.platforms) do
        for _, variant in ipairs(target.settings.variants) do
            local variant_settings = target.settings.settings_by_variant[variant];
            
            local output = "";
            if target:prototype() == build.Executable then
                output = native( relative(root(("../%s_%s/bin/%s.exe"):format(platform, variant, target:id()))) );
            end
            
            local defines = {
                ('/DBUILD_PLATFORM_%s'):format( upper(platform) );
                ('/DBUILD_VARIANT_%s'):format( upper(variant) );
                ('/DBUILD_LIBRARY_SUFFIX="\\"_%s.lib\\""'):format( target.architecture );
                ('/DBUILD_LIBRARY_TYPE_%s'):format( upper(variant_settings.library_type) );
            };
            if variant_settings.defines then
                for _, define in ipairs(variant_settings.defines) do
                    table.insert( defines, define );
                end
            end    
            if target.defines then
                for _, define in ipairs(target.defines) do
                    table.insert( defines, define );
                end
            end            
            defines = string.gsub( table.concat(defines, ";"), '"', '&quot;' );

            local include_directories = {};
            for _, directory in ipairs(target.settings.include_directories) do
                table.insert( include_directories, native(directory) );
            end
            include_directories = table.concat( include_directories, ";" );

            local output_directory = native( relative(root(("%s_%s/obj"):format(platform, variant))) );
            local intermediate_directory = native( relative(root(("%s_%s/obj"):format(platform, variant))) );
            local build_command = ("%s variant=%s platform=%s"):format( build_tool, variant, platform );

            write( PROPERTY_GROUP, 
                platform, variant,
                output_directory,
                intermediate_directory,
                build_command,
                build_command,
                build_command,
                output,
                defines,
                include_directories
            );
        end
    end
end

local function generate_files( target, write, files )
    local START_FILES = [[
  <ItemDefinitionGroup>
  </ItemDefinitionGroup>
  <ItemGroup>
]];

    local FILE = [[
    <ClCompile Include="%s" />
]];

    local FINISH_FILES = [[
  </ItemGroup>
]];

    write( START_FILES );
    for _, file in ipairs(files) do 
        write( FILE, relative(file) );
    end
    write( FINISH_FILES );
end

local function generate_footer( target, write )
    local FOOTER = [[
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>
]];
    write( FOOTER );
end

local function write_function( file )
    return function( format, ... )
        file:write( format:format(...) );
    end
end

-- Generate a Visual Studio `.vcxproj` project file for a target and files.
function vcxproj.generate( target, files )
  local filename = ("%s.vcxproj"):format( target:path() );
  print( leaf(filename) );
    pushd( target:working_directory():path() );
    local file = io.open( filename, "wb" )
    assertf( file, "Opening '%s' to write project failed", filename );
    local write = write_function( file );
    generate_header( target, write );
    generate_project_configurations( target, write );
    generate_globals_property_group( target, write );
    generate_configuration_property_groups( target, write );
    generate_import( target, write );
    generate_project_file_version_property_group( target, write );
    generate_property_groups( target, write );
    generate_files( target, write, files );
    generate_footer( target, write );
    file:close();
    file = nil;
    popd();
end

return vcxproj;
