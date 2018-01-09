
local Copy = build.TargetPrototype( "Copy" );

function Copy.build( copy )
    build.rm( copy );
    build.cp( copy, copy:dependency() );
end
