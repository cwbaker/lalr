
HeaderFilePrototype = TargetPrototype {
	"HeaderFile", BIND_SOURCE_FILE
}

function HeaderFile( header_file )
	assert( type(header_file) == "string" );
	return target( header_file, HeaderFilePrototype );
end
