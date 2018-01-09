
local Css = build:TargetPrototype( "sass.Css" );

function Css.build( css )
    local settings = css.settings;

    local load_paths = {};
    if settings.sass.load_paths then
        for _, directory in ipairs(settings.load_paths) do
            table.insert( load_paths, ('-I "%s"'):format(directory) );
        end
    end
    if css.load_paths then
        for _, directory in ipairs(css.load_paths) do
            table.insert( load_paths, ('-I "%s"'):format(directory) );
        end
    end

    local ruby = settings.ruby.executable;
    local arguments = {
        ruby,
        settings.sass.executable,
        table.concat( load_paths, " " ),
        ('--cache-location "%s"'):format( build:interpolate("${obj}/.sass-cache", settings) ),
        ('"%s"'):format( css:dependency():filename() ),
        ('"%s"'):format( css:filename() ),
    };
    local environment = {
        PATH = os.getenv( "PATH" );
    };
    build:system( ruby, arguments, environment, build:dependencies_filter(css) );
end
