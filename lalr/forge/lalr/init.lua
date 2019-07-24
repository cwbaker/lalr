
local lalr = {};

function lalr.configure( toolset, lalr_settings )
	return {
		lalrc = lalr_settings.lalrc or '${bin}/lalrc';
	};
end

function lalr.initialize( toolset )
	if toolset:configure(lalr, 'lalr') then
		local Lalrc = require 'forge.lalr.Lalrc';
		toolset.Lalrc = forge:PatternElement( Lalrc );
		return toolset;
	end
end

return lalr;
