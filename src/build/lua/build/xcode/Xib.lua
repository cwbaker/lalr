
local Xib = build:TargetPrototype( "xcode.Xib" );

function Xib.build( xib )
    local command_line = {
        'xcrun';
        ('--sdk %s'):format( ios.sdkroot_by_target_and_platform(xib, platform) );
        'ibtool';
        '--output-format binary1';
        ('--compile "%s"'):format( xib );
        ('"%s"'):format( xib:dependency() );
    };
    build:system( 
        xib.settings.ios.xcrun, 
        command_line
    );
end

xcode.Xib = Xib;
