
local Lipo = build.TargetPrototype( "xcode.Lipo" );

function Lipo.build( lipo )
    lipo_executable( lipo );
end

xcode.Lipo = Lipo;
