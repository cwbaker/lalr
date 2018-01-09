
local Target = build.Target;

setmetatable( Target, {
    __call = function( _, build, identifier, target_prototype )
        local target_ = build:target( identifier, target_prototype );
        getmetatable( target_ ).__call = function( target, ... )
            target:depend( ... );
            return target;
        end;
        return target_;
    end
} );

function Target:depend( dependencies )
    local settings = self.settings;
    if type(dependencies) == "string" then
        local source_file = build:SourceFile( dependencies, settings );
        self:add_dependency( source_file );
    elseif type(dependencies) == "table" then
        build:merge( self, dependencies );
        for _, value in ipairs(dependencies) do 
            local source_file = build:SourceFile( value, settings );
            self:add_dependency( source_file );
        end
    end
    return self;
end

function Target:implicit_depend( dependencies )
    local settings = self.settings;
    if type(dependencies) == "string" then
        local source_file = build:SourceFile( dependencies, settings );
        self:add_implicit_dependency( source_file );
    elseif type(dependencies) == "table" then
        build:merge( self, dependencies );
        for _, value in ipairs(dependencies) do 
            local source_file = build:SourceFile( value, settings );
            self:add_implicit_dependency( source_file );
        end
    end
    return self;
end
