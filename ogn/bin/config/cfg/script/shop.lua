dofile("../config/cfg/script/global.lua");

function OnSellProduct(userId, productInsId)
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg("����:"..productInsId);
	return 0;
end