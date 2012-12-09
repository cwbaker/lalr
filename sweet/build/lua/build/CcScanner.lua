
local function local_include( target, match )
    local header = HeaderFile( "%s%s" % {target:directory(), match} );
    target:add_dependency( header );
    if header:get_bind_type() ~= BIND_INTERMEDIATE_FILE then
        scan( header, CcScanner );
    end
end

local function global_include( target, match )
    local filename = root( "../%s" % match );
    if exists(filename) then
        local header = HeaderFile( filename );
        target:add_dependency( header );
        if header:get_bind_type() ~= BIND_INTERMEDIATE_FILE then
            scan( header, CcScanner );
        end
    end
end

CcScanner = Scanner {
    [ [[^#include "([^"\n\r]*)"]] ] = local_include;
    [ [[^#include <([^>\n\r]*)>]] ] = global_include;
}
