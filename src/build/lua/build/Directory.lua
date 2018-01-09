
DirectoryPrototype = TargetPrototype { "Directory", BIND_DIRECTORY };

function DirectoryPrototype.build( directory )
    if directory:is_outdated() then
        mkdir( directory:get_filename() );
    end
end

function Directory( directory )
	return target( directory, DirectoryPrototype );
end
