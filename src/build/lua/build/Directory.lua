
Directory = build.TargetPrototype( "Directory" );

function Directory.create( settings, id )
	return build.File( id, Directory );
end

function Directory.build( directory )
    build.mkdir( directory:filename() );
end

function Directory.clean( directory )
end
