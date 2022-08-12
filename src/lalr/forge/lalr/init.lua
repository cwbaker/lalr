
local lalr = {};

function lalr.configure( toolset, lalr_settings )
	return {
		lalrc = lalr_settings.lalrc or '${bin}/lalrc';
	};
end

function lalr.install( toolset )
    toolset:configure_once( 'lalr', lalr.configure );
	toolset.Lalrc = require( 'forge.lalr.Lalrc' );
	return true;
end

return lalr;
