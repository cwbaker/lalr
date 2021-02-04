
local lalr = ToolsetPrototype( 'lalr' );

function lalr.configure( toolset, lalr_settings )
	return {
		lalrc = lalr_settings.lalrc or '${bin}/lalrc';
	};
end

function lalr.initialize( toolset )
	toolset.Lalrc = require( 'forge.lalr.Lalrc' );
	return true;
end

return lalr;
