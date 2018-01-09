
local R = build.TargetPrototype( "android.R" );

function R.create( settings, packages )
    local r = build.Target( build.anonymous(), R );
    r.settings = settings;
    r.packages = packages;
    for index, package in ipairs(packages) do 
        local filename = ("%s/R.java"):format( package:gsub("%.", "/") );
        r:set_filename( build.generated(filename), index );
    end
    return r;
end

function R:build()
    local android_manifest = self:dependency( 1 );
    assertf( android_manifest and build.leaf(android_manifest) == "AndroidManifest.xml", "Android R '%s' does not specify 'AndroidManifest.xml' as its first dependency", self:path() );

    local settings = self.settings;
    local working_directory = self:working_directory();
    local gen_directory = ("%s/%s"):format( settings.gen, build.relative(working_directory:path(), build.root()) );

    local command_line = {
        'aapt',
        'package',
        '--auto-add-overlay',
        '-f',
        '-m',
        ('-I "%s"'):format( android.android_jar(settings) ),
        ('-J "%s"'):format( gen_directory ),
        ('-M "%s"'):format( android_manifest:filename() ),
        ('--extra-packages %s'):format(table.concat(self.packages, ":"))
    };

    for _, dependency in self:dependencies( 2 ) do
        table.insert( command_line, ('-S "%s"'):format(build.relative(dependency)) );
    end

    local aapt = ('%s/aapt'):format( settings.android.build_tools_directory );
    build.system( aapt, command_line );
end

android.R = R;
