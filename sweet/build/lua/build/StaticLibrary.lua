
StaticLibrary = Rule( "StaticLibrary", BIND_GENERATED_FILE );

function StaticLibrary:load()
    load_module( self );
    self:set_filename( self.settings.lib.."/"..lib_name(self:id()) );
end

function StaticLibrary:static_depend()
    if built_for_platform_and_variant(self) then
        self:add_dependency( Directory(self.settings.lib) );
    end
end

function StaticLibrary:build()
    if self:is_outdated() and built_for_platform_and_variant(self) then
        build_library( self );
    end    
end

function StaticLibrary:clean()
    if built_for_platform_and_variant(self) then
        clean_library( self );
    end
end
