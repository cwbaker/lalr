
local Lalrc = forge:TargetPrototype( 'Lalrc' );

function Lalrc.create( forge, identifier )
    local target = forge:File( identifier, Lalrc );
    target:add_implicit_dependency( forge:file(forge.settings.lalr.lalrc) );
    return target;
end

function Lalrc.build( forge, target )
    local lalrc = forge.settings.lalr.lalrc;
    forge:system( lalrc, ('lalrc -o "%s" "%s"'):format(target, target:dependency()) );
    target:add_implicit_dependency( forge:file(lalrc) );
end
