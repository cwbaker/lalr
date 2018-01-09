
CompilePrototype = TargetPrototype { "Compile", BIND_PHONY };

function CompilePrototype.static_depend( compile )
    local definition = compile.definition;
    if build.built_for_platform_and_variant(definition) then
        local directory = Directory( obj_directory(definition) );

        local pch = nil;
        -- if compile.pch then
        --     precompiled_header = PrecompiledHeader( obj_directory(compile)..obj_name(compile.pch) );
        --     precompiled_header.header = compile.pch;
        --     precompiled_header.unit = compile;
        --     precompiled_header:add_dependency( directory );
        --     compile:add_dependency( precompiled_header );
        --     compile.precompiled_header = precompiled_header;
        -- end
        
        for _, value in ipairs(definition) do
            local source = SourceFile( value );
            source:set_required_to_exist( true );
            source.unit = compile;
            local object = File( "%s/%s" % {obj_directory(definition), obj_name(value, compile.architecture)} );
            object.source = value;
            source.object = object;
            object:add_dependency( source );
            object:add_dependency( directory );
            object:add_dependency( pch );           
            compile:add_dependency( object );
        end
    end
end;

function CompilePrototype.depend( compile )
    local definition = compile.definition;
    if build.built_for_platform_and_variant(definition) then
        for _, value in ipairs(definition) do
            local source = SourceFile( value );
            assert( source, "Failed to find source file '%s' for scanning" % tostring(value) );
            scan( source, CcScanner );
        end
    end
end;

function CompilePrototype.build( compile )
    local definition = compile.definition;
    if compile:is_outdated() and build.built_for_platform_and_variant(definition) then
        cc( compile, definition );
    end
end;

function CompilePrototype.clean( compile )
    local definition = compile.definition;
    if build.built_for_platform_and_variant(definition) then
        for dependency in compile:get_dependencies() do
            if dependency:prototype() == FilePrototype then
                rm( dependency:path() );
            end
        end
    end
end;

function Compile( source )
    assert( type(source) == "table" and source:prototype() == SourcePrototype );
    return target( "", CompilePrototype, {definition = source} );
end
