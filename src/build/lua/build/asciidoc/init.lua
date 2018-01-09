
asciidoc = {};

function asciidoc.configure( settings )
    local local_settings = build.local_settings;
    if not local_settings.asciidoc then
        local_settings.updated = true;
        local_settings.asciidoc = {};
    end
end

function asciidoc.initialize( settings )
end

require "build.asciidoc.Html";
require "build.asciidoc.Pdf";

build:register_module( asciidoc );
