
Directory = Rule( "Directory", BIND_DIRECTORY );

function Directory:build()
    if self:is_outdated() then
        mkdir( self:get_filename() );
    end
end
