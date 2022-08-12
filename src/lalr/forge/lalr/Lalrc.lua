
local Lalrc = PatternRule( 'Lalrc' );

function Lalrc.created( toolset, target )
    local lalrc = toolset:interpolate( toolset.lalr.lalrc );
    target:add_dependency( toolset:Target(lalrc) );
end

function Lalrc.build( toolset, target )
    local lalrc = target:dependency(1);
    local filename = target:dependency(2):filename();
    printf( leaf(filename) );
    system( lalrc, ('lalrc -o "%s" "%s"'):format(target, filename) );
end

return Lalrc;
