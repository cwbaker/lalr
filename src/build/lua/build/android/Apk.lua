
local Apk = build:TargetPrototype( "android.Apk" );

function Apk:build()
    local android_manifest = self:dependency( 1 );
    assertf( android_manifest and build:leaf(android_manifest:filename()) == "AndroidManifest.xml", "Android APK '%s' does not specify 'AndroidManifest.xml' as its first dependency'", self:path() );

    local settings = self.settings;
    local aapt = ("%s/aapt"):format( settings.android.build_tools_directory );
    local resources = table.concat( settings.resources, " -S " );
    local android_jar = ("%s/platforms/%s/android.jar"):format( settings.android.sdk_directory, settings.android.sdk_platform );
    build:system( aapt, {
        'aapt',
        'package',
        '--auto-add-overlay',
        '-f',
        ('-M "%s"'):format( android_manifest ),
        ('-S %s'):format( resources ),
        ('-I "%s"'):format( android_jar ),
        ('-F "%s.unaligned"'):format( self )
    } );

    build:pushd( ("%s/%s"):format(build:branch(self), build:basename(self)) );
    for dependency in build:walk_dependencies(self, 2) do 
        build:system( aapt, {
            "aapt",
            'add',
            ('-f "%s.unaligned"'):format( build:relative(self) ),
            ('"%s"'):format( build:relative(dependency) )
        } );
    end

    local jarsigner = ("%s/bin/jarsigner"):format( settings.java.jdk_directory );
    local key = _G.key or "androiddebugkey";
    local keypass = _G.keypass or "android";
    local keystore = _G.keystore or build:relative( ("%s/debug.keystore"):format(self:working_directory():path()) );
    build:system( jarsigner, {
        'jarsigner',
        '-sigalg MD5withRSA',
        '-digestalg SHA1',
        ('-keystore %s'):format( keystore ),
        ('-storepass %s'):format( keypass ),
        ('%s.unaligned'):format( build:relative(self) ),
        ('%s'):format( key )
    } );

    local zipalign = ("%s/zipalign"):format( settings.android.build_tools_directory );
    build:system( zipalign, {
        'zipalign',
        '-f 4',
        ('%s.unaligned'):format( build:relative(self) ),
        ('%s'):format( build:relative(self) )
    } );
    build:popd();
end

function Apk:clean()
    build:rm( ("%s.unaligned"):format(self:filename()) );
    build:rm( self );
end

android.Apk = Apk;
