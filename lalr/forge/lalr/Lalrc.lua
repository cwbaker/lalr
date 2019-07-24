
local Lalrc = forge:TargetPrototype( 'Lalrc' );

function Lalrc.create( toolset, identifier )
    local target = toolset:File( identifier, Lalrc );
    local lalrc = toolset:interpolate( toolset.settings.lalr.lalrc );
    target:add_dependency( toolset:Target(lalrc) );
    target:set_cleanable( false );
    return target;
end

function Lalrc.build( toolset, target )
    local lalrc = target:dependency(1);
    local filename = target:filename();
    printf( leaf(filename) );
    system( lalrc, ('lalrc -o "%s" "%s"'):format(filename, target:dependency(2)) );
end

return Lalrc;
