
local Lalrc = forge:PatternPrototype( 'Lalrc' );

function Lalrc.created( toolset, target )
    local lalrc = toolset:interpolate( toolset.settings.lalr.lalrc );
    target:add_dependency( toolset:Target(lalrc) );
end

function Lalrc.build( toolset, target )
    local lalrc = target:dependency(1);
    local filename = target:filename();
    printf( leaf(filename) );
    system( lalrc, ('lalrc -o "%s" "%s"'):format(filename, target:dependency(2)) );
end

return Lalrc;
