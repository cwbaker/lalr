
local QtMoc = build:TargetPrototype( "qt.QtMoc" );

function QtMoc:build()
    local moc = self.settings.qt.moc;
    local command_line = {
        'moc',
        ('-o "%s"'):format( build:relative(self) ),
        ('"%s"'):format( self:dependency() )
    };
    build:system( moc, command_line );
end
