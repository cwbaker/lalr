
ArchivePrototype = TargetPrototype { "Archive", BIND_GENERATED_FILE };

function ArchivePrototype.load( archive )
    local definition = archive.definition;
    if build.built_for_platform_and_variant(definition) then
        archive.architecture = archive.architecture or "";
        archive:set_filename( archive:path() );
    end
end;

function ArchivePrototype.static_depend( archive )
    local definition = archive.definition;
    if build.built_for_platform_and_variant(definition) then
        archive:add_dependency( Directory(branch(archive:get_filename())) );

        for _, value in ipairs(definition) do
            if type(value) == "table" then
                assert( value.clone and type(value.clone) == "function", "Tables with integer keys in Archives must be clonable targets" );
                local target = value:clone();
                target.architecture = archive.architecture;
                target.module = archive.module;
                archive:add_dependency( target );
            end
        end
    end
end;

function ArchivePrototype.build( archive )
    local definition = archive.definition;
    if archive:is_outdated() and build.built_for_platform_and_variant(definition) then
        build_library( archive, definition );
    end    
end;

function ArchivePrototype.clean( archive )
    local definition = archive.definition;
    if build.built_for_platform_and_variant(definition) then
        clean_library( archive, definition );
    end
end;

function Archive( id, definition )
    assert( type(definition) == "table" );
    return target( id, ArchivePrototype, {definition = definition} );
end
