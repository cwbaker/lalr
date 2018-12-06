
local Lalrc = forge:TargetPrototype( 'Lalrc' );

function Lalrc.create( forge, identifier )
    local target = forge:File( identifier, Lalrc );
    local lalrc = forge:interpolate( forge.settings.lalr.lalrc );
    target:add_dependency( forge:Target(lalrc) );
    target:set_cleanable( false );
    return target;
end

function Lalrc.build( forge, target )
    local lalrc = target:dependency(1);
    local filename = target:filename();
    printf( forge:leaf(filename) );
    forge:system( lalrc, ('lalrc -o "%s" "%s"'):format(filename, target:dependency(2)) );
end

return Lalrc;
