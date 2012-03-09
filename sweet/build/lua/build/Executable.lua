
Executable = Rule( "Executable", BIND_GENERATED_FILE );

function Executable:load()
    load_module( self );
    self:set_filename( self.settings.bin.."/"..exe_name(self:id()) );
end

function Executable:static_depend()
    if built_for_platform_and_variant(self) then
        self:add_dependency( Directory(self.settings.bin) );

        local libraries = {};
        if self.libraries then
            for _, value in ipairs(self.libraries) do
                local library = find_target( root(value) );
                assert( library, "Failed to find library '"..value.."'" );
                if built_for_platform_and_variant(library) then
                    table.insert( libraries, library );
                    self:add_dependency( library );
                end
            end
        end
        self.libraries = libraries;
    end
end

function Executable:build()
    if self:is_outdated() and built_for_platform_and_variant(self) then
        build_executable( self );
    end
end

function Executable:clean()
    if built_for_platform_and_variant(self) then
        clean_executable( self );
    end
end

function Executable:project()
    generate_visual_studio_project( self );
end

