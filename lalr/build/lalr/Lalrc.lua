
local Lalrc = build:TargetPrototype( "Lalrc" );

function Lalrc.build( build, target )
    local lalrc = target.settings.lalr.lalrc;
    build:system( lalrc, ('lalrc -o "%s" "%s"'):format(target, target:dependency()) );
end
