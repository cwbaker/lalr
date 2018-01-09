
local TargetPrototype = build.TargetPrototype;

setmetatable( TargetPrototype, {
    __call = function( _, build, identifier )
        -- Split a '.' delimited string into a table hierarchy returning the last 
        -- level table and identifier.
        local function split_modules( module, qualified_id )
            local start = qualified_id:find( ".", 1, true );
            if start then
                local identifier = qualified_id:sub( 1, start - 1 );
                local submodule = module[identifier];
                if submodule == nil then 
                    submodule = {};
                    module[identifier] = submodule;
                end
                local remaining = qualified_id:sub( start + 1 );
                return split_modules( submodule, remaining );
            end
            return module, qualified_id;
        end

        local function create( target_prototype, build, identifier )
            local settings = build:current_settings();
            local identifier = build:interpolate( identifier, settings );
            local target = build:Target( identifier, target_prototype );
            if build:is_relative(identifier) then 
                target:set_filename( build:object(identifier) );
            else
                target:set_filename( target:path() );
            end
            target:set_cleanable( true );
            target:add_ordering_dependency( build:Directory(build:branch(target)) );
            target.settings = settings;
            return target;
        end

        local target_prototype = build:target_prototype( identifier );
        getmetatable( target_prototype ).__call = function( target_prototype, build, ... )
            local target;
            local create_function = target_prototype.create;
            if create_function then 
                local settings = build:current_settings();
                target = create_function( settings, ... );
            else
                target = create( target_prototype, build, ... );
            end
            local annotate_function = target.annotate;
            if annotate_function then 
                annotate_function( target, build, ... );
            end
            return target;
        end;

        local module, identifier = split_modules( build, identifier );
        module[identifier] = target_prototype;
        build[identifier] = target_prototype;
        return target_prototype;
    end
} );
