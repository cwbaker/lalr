
local lalr = {};

function lalr.configure( forge, lalr_settings )
	return {
		lalrc = lalr_settings.lalrc or '${bin}/lalrc';
	};
end

function lalr.initialize( forge )
	if forge:configure(lalr, 'lalr') then
		local Lalrc = require 'forge.lalr.Lalrc';
		forge.Lalrc = forge:PatternElement( Lalrc );
		return forge;
	end
end

return lalr;
