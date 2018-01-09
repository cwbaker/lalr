
local App = build.TargetPrototype( "ios.App" );

local function default_identifier_filename( identifier, architecture, settings )
    local settings = settings or build.current_settings();
    local identifier = build.interpolate( identifier, settings );
    local leaf = build.leaf( identifier );
    local branch = settings.bin;
    if build.is_absolute(identifier) then 
        branch = build.branch( identifier );
    end
    local identifier = ("%s$$"):format( identifier );
    local filename = ("%s/%s"):format( branch, leaf );
    return identifier, filename;
end

function App.create( settings, identifier )
    local identifier, filename = default_identifier_filename( identifier, architecture, settings );
    local app = build.Target( identifier, App );
    app:set_filename( filename );
    app:set_cleanable( true );
    app:add_ordering_dependency( build.Directory(build.branch(app)) );
    app.settings = settings;
    app.architecture = architecture or settings.default_architecture;
    return app;
end

function App:depend( dependencies )
    local entitlements = dependencies.entitlements;
    if entitlements then 
        self.entitlements = ("%s/%s"):format( obj_directory(self), "Entitlements.plist" );
        table.insert( dependencies, build.Generate (self.entitlements) (entitlements) );
    end
    return build.default_depend( self, dependencies );
end

function App.build( app )
    local xcrun = app.settings.ios.xcrun;
    if app.settings.generate_dsym_bundle then 
        local executable;
        for _, dependency in app:dependencies() do 
            if dependency:prototype() == build.xcode.Lipo then 
                executable = dependency:filename();
                break;
            end
        end
        if executable then 
            build.system( xcrun, ('xcrun dsymutil -o "%s.dSYM" "%s"'):format(app:filename(), executable) );
            if app.settings.strip then 
                build.system( xcrun, ('xcrun strip "%s"'):format(executable) );
            end
        end
    end

    local provisioning_profile = _G.provisioning_profile or app.settings.provisioning_profile;
    if provisioning_profile then
        local embedded_provisioning_profile = ("%s/embedded.mobileprovision"):format( app:filename() );
        build.rm( embedded_provisioning_profile );
        build.cp( embedded_provisioning_profile, provisioning_profile );
    end

    if platform == "ios" then
        local command_line = {
            "codesign";
            ('-s "%s"'):format( _G.signing_identity or app.settings.ios.signing_identity );
            "--force";
            "--no-strict";
            "-vv";
            ('"%s"'):format( app );
        };
        local entitlements = app.entitlements;
        if entitlements then 
            table.insert( command_line, ('--entitlements "%s"'):format(entitlements) );
        end

        local codesign = app.settings.ios.codesign;
        build.system( codesign, table.concat(command_line, " "), environment );
    end
end

function App.clean( app )
    build.rmdir( app:filename() );
end

ios.App = App;
