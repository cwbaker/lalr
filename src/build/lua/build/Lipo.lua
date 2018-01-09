
LipoPrototype = TargetPrototype { "Lipo", BIND_GENERATED_FILE };

function LipoPrototype.load( lipo )
    local definition = lipo.definition;
    if build.built_for_platform_and_variant(definition) then
        lipo:set_filename( lipo:path() );
        local architectures = definition.architectures or definition.settings.architectures;
        for _, architecture in ipairs(architectures) do 
            local link = Link( "%s/%s_%s" % {definition.settings.obj, lipo:id(), architecture}, definition );
            link.architecture = architecture;
            link.module = lipo.module;
            lipo:add_dependency( link );
        end
    end
end;

function LipoPrototype.static_depend( lipo )
    local definition = lipo.definition;
    if build.built_for_platform_and_variant(definition) then
        lipo:add_dependency( Directory(branch(lipo:get_filename())) );
    end
end;

function LipoPrototype.build( lipo )
    local definition = lipo.definition;
    if lipo:is_outdated() and build.built_for_platform_and_variant(definition) then
        lipo_executable( lipo, definition );
    end
end;

function LipoPrototype.clean( lipo )
    local definition = lipo.definition;
    if build.built_for_platform_and_variant(definition) then
        clean_executable( lipo );
    end
end;

function Lipo( id, definition )
    assert( type(definition) == "table" );
    return target( id, LipoPrototype, {definition = definition} );
end
