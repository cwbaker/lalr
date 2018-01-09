
local Lipo = build:TargetPrototype( "xcode.Lipo" );

function Lipo.build( lipo )
    local settings = lipo.settings;
    settings.lipo_executable( lipo );
end

xcode.Lipo = Lipo;
