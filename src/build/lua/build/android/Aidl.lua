
local Aidl = build.TargetPrototype( "android.Aidl" );

function Aidl:build()
    local settings = self.settings;
    local aidl = ("%s/aidl"):format( settings.android.build_tools_directory );
    local platform = ("%s/platforms/%s/framework.aidl"):format( settings.android.sdk_directory, settings.android.sdk_platform );
    local output = build.generated(self:working_directory());
    local command_line = {
        'aidl',
        ('-p%s'):format( platform ),
        ('-o"%s"'):format( output ), 
        ('"%s"'):format( self:dependency() )
    };

    build.system( 
        aidl, 
        command_line
    );
end

android.Aidl = Aidl;
