
local Lalrc = forge:TargetPrototype( 'Lalrc' );

function Lalrc.create( forge, identifier )
    local target = forge:File( identifier, Lalrc );
    local lalrc = forge:interpolate( forge.settings.lalr.lalrc );
    target:add_implicit_dependency( forge:Target(lalrc) );
    target:set_cleanable( false );
    return target;
end

function Lalrc.build( forge, target )
    local lalrc = target:implicit_dependency();
    forge:system( lalrc, ('lalrc -o "%s" "%s"'):format(target, target:dependency()) );
    local lalrc = forge:interpolate( forge.settings.lalr.lalrc );
    target:add_implicit_dependency( forge:Target(lalrc) );
end
