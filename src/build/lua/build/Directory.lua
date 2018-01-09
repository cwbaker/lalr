
local Directory = build:TargetPrototype( "Directory" );

function Directory.create( settings, identifier )
	return build:File( identifier, Directory );
end

function Directory:build()
    build:mkdir( self );
end

function Directory:clean()
end
