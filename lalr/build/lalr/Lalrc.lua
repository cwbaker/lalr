
local Lalrc = build:TargetPrototype( "Lalrc" );

function Lalrc.create( build, settings, identifier )
    local settings = build:current_settings();
    local identifier = build:interpolate( identifier, settings );
    local target = build:File( identifier, Lalrc );
    target:add_ordering_dependency( build:Directory(build:branch(target)) );
    target:add_implicit_dependency( build:file(settings.lalr.lalrc) );
    target.settings = settings;
    return target;
end

function Lalrc.build( build, target )
    local lalrc = target.settings.lalr.lalrc;
    build:system( lalrc, ('lalrc -o "%s" "%s"'):format(target, target:dependency()) );
    target:add_implicit_dependency( build:file(lalrc) );
end
