
local Pdf = build:TargetPrototype( "asciidoc.Pdf" );

function Pdf.build( pdf )
    local settings = pdf.settings;
    local python = settings.python.executable;
    local arguments = {
        "python",
        settings.a2x.executable,
        "-f pdf",
        ('-o "%s"'):format( pdf ),
        ('"%s"'):format( pdf:dependency() )
    };
    build:system( python, arguments, nil, build:dependencies_filter(pdf) );
end;
