
SourcePrototype = TargetPrototype { "Source", BIND_PHONY };

function SourcePrototype.clone( source )
    return Compile( source );
end

function Source( source )
    assert( source == nil or type(source) == "table" );
    return target( "", SourcePrototype, source );
end
