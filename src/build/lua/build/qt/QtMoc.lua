
local QtMoc = build.TargetPrototype( "qt.QtMoc" );

function QtMoc.create( settings, definition )
    assert( definition and type(definition) == "table" );

    local cxx_files = {};
    for _, value in ipairs(definition) do
        local cxx_file = ("moc_%s.cpp"):format( build.basename(value) );

        local qtmoc = build.File( cxx_file, QtMoc, {value} );
        qtmoc:set_required_to_exist( false );
        table.insert( cxx_files, qtmoc );

        local header = build.SourceFile( value );
        qtmoc:add_dependency( header );
    end
    return table.unpack( cxx_files );
end

function QtMoc.build( qtmoc )
    local filename = qtmoc:filename();
    if not build.exists(filename) or qtmoc:outdated() then
        local moc = build.settings.qt.moc;
        build.system( moc, ('moc %s -o %s'):format(qtmoc[1], build.relative(filename)) );
    end
end
