
PrecompiledHeaderPrototype = TargetPrototype {
	"PrecompiledHeader", BIND_GENERATED_FILE,

	depend = function( self )
	    local source = SourceFile( cxx_name(self.header) );
	    source:set_required_to_exist( true );
	    source.unit = self.unit;
	    source.object_file = self;
	    scan( source, CcScanner );
	    
	    self.source = source:id();
	    self:add_dependency( source );
	    self:add_dependency( directory );
	end;
}

function PrecompiledHeader( precompiled_header )
	assert( type(precompiled_header) == "string" );
	return target( precompiled_header, PrecompiledHeaderPrototype );
end
