
FilePrototype = TargetPrototype {
	"File", BIND_GENERATED_FILE
}

function File( file )
	assert( type(file) == "string" );
	return target( file, FilePrototype );
end
