
Project = Rule( "Project", BIND_GENERATED_FILE );

function Project:load()
    local filename = string.format( "%s.vcproj", basename(self:id()) );
    self:set_filename( absolute(filename) );
    
    local working_directory = self:get_working_directory();
    for target in working_directory:get_targets() do
        if string.match(target:id(), ".*%.build")  then
            self:add_dependency( target );
        end            
    end
end

function Project:generate()
    if self:is_outdated() then
        generate_visual_studio_project( self:get_filename(), self.module );
    end
end
