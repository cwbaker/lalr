
CcScanner = Scanner {
    [ [[^#include "([^"\n\r]*)"]] ] = function( target, match )
        local header = HeaderFile( target:directory()..match );
        target:add_dependency( header );
        if header:get_bind_type() ~= BIND_INTERMEDIATE_FILE then
            scan( header, CcScanner );
        end
    end;

    [ [[^#include <([^>\n\r]*)>]] ] = function( target, match )
        local filename = root( "../"..match );
        if exists(filename) then
            local header = HeaderFile( filename );
            target:add_dependency( header );
            if header:get_bind_type() ~= BIND_INTERMEDIATE_FILE then
                scan( header, CcScanner );
            end
        end
    end;
}
