
LinkPrototype = TargetPrototype { "Link", BIND_GENERATED_FILE };

function LinkPrototype.load( link )
    local definition = link.definition;
    if build.built_for_platform_and_variant(definition) then
        link.architecture = link.architecture or "";
        link:set_filename( link:path() );
    end
end;

function LinkPrototype.static_depend( link )
    local definition = link.definition;
    if build.built_for_platform_and_variant(definition) then
        link:add_dependency( Directory(branch(link:get_filename())) );

        local libraries = {};
        if definition.libraries then
            for _, value in ipairs(definition.libraries) do
                local library = find_target( root(value) );
                assert( library, "Failed to find library '%s'" % value );
                if build.built_for_platform_and_variant(library) then
                    for archive in library:get_dependencies() do
                        if archive.architecture == link.architecture then
                            table.insert( libraries, archive );
                            link:add_dependency( archive );
                        end
                    end
                end
            end
        end
        link.libraries = libraries;

        for _, value in ipairs(definition) do
            if type(value) == "table" then
                assert( value.clone and type(value.clone) == "function", "Tables with integer keys in Archives must be clonable targets" );
                local target = value:clone();
                target.architecture = link.architecture;
                target.module = link.module;
                link:add_dependency( target );
            end
        end
    end
end;

function LinkPrototype.build( link )
    local definition = link.definition;
    if link:is_outdated() and build.built_for_platform_and_variant(definition) then
        build_executable( link, definition );
    end
end;

function LinkPrototype.clean( link )
    local definition = link.definition;
    if build.built_for_platform_and_variant(definition) then
        clean_executable( link, definition );
    end
end;

function Link( id, definition )
    assert( type(definition) == "table" );
    return target( id, LinkPrototype, {definition = definition} );
end
