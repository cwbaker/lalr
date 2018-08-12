
local Lalrc = forge:TargetPrototype( 'Lalrc' );

function Lalrc.create( forge, settings, identifier )
    local target = forge:File( identifier, Lalrc, settings );
    target:add_ordering_dependency( forge:Directory(forge:branch(target)) );
    target:add_implicit_dependency( forge:file(settings.lalr.lalrc) );
    target.settings = settings;
    return target;
end

function Lalrc.build( forge, target )
    local lalrc = target.settings.lalr.lalrc;
    forge:system( lalrc, ('lalrc -o "%s" "%s"'):format(target, target:dependency()) );
    target:add_implicit_dependency( forge:file(lalrc) );
end
