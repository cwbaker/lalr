
SourceFilePrototype = TargetPrototype {
	"SourceFile", BIND_SOURCE_FILE
}

function SourceFile( source_file )
	assert( type(source_file) == "string" );
	return target( source_file, SourceFilePrototype );
end
