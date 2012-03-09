
AsciiDoc = Rule( "AsciiDoc", BIND_GENERATED_FILE );

function AsciiDoc:load()
    if self.settings then
        setmetatable( self.settings, {__index = settings} );
    else
        self.settings = settings;
    end
end

function AsciiDoc:static_depend()
    local source = SourceFile( self[1] );
    source:set_required_to_exist( true );
    self.source = self[1];
    self:add_dependency( source );
end

function AsciiDoc:document()
    if self:is_outdated() and built_for_platform_and_variant(self) then
        local python = self.settings.python.executable;
        local asciidoc = self.settings.asciidoc.executable;
        local conf_file = self.settings.asciidoc.conf_file;
        print( self.source );
        local arguments = "python %s -f %s %s" % {asciidoc, conf_file, self.source};
        if execute(python, arguments) ~= 0 then
            error( arguments.." failed" );
        end
    end
end
