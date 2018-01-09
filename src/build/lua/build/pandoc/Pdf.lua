
local Pdf = build.TargetPrototype( "pandoc.Pdf" );

function Pdf.build( pdf )
    local settings = pdf.settings;
    local pandoc = "/usr/local/bin/pandoc";
    local arguments = {
        "pandoc",
        "--to=pdf",
        ('-o "%s"'):format( pdf ),
        ('"%s"'):format( pdf:dependency() )
    };
    local environment = {
        PATH = os.getenv( "PATH" );
    };
    build.system( pandoc, arguments, environment, build.dependencies_filter(html) );
end
