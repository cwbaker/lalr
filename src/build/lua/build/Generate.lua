
local Generate = build:TargetPrototype( "Generate" );

function Generate:build()
    local outputs = {};
    for _, dependency in self:dependencies() do 
        local template = assert( loadfile(build:native(build:absolute(dependency))) );
        local success, output_or_error_message = pcall( template, self );
        assert( success, output_or_error_message );
        table.insert( outputs, output_or_error_message );
    end

    local output_file, error_message = io.open( self:filename(), "wb" );
    assert( output_file, error_message );
    output_file:write( table.concat(outputs, "") );
    output_file:close();
    output_file = nil;
end
