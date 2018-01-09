
local Html = build:TargetPrototype( "asciidoc.Html" );

function Html.build( html )
    local settings = html.settings;
    local python = settings.python.executable;
    local arguments = {
        "python",
        settings.asciidoc.executable,
        ('-f "%s"'):format( settings.asciidoc.conf_file ),
        ('-o "%s"'):format( html ),
        ('"%s"'):format( html:dependency() )
    };
    local environment = {
        PATH = os.getenv( "PATH" );
    };
    build:system( python, arguments, environment, build:dependencies_filter(html) );
end
