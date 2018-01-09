
local Plist = build:TargetPrototype( "xcode.Plist" );

function Plist.build( plist )
    local command_line = {
        'plutil';
        '-convert binary1';
        ('-o "%s"'):format( plist );
        ('"%s"'):format( plist:dependency() );
    };
    local plutil = plist.settings.ios.plutil;
    build:system( plutil, table.concat(command_line, " ") );
end

xcode.Plist = Plist;
