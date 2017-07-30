dofile("../config/cfg/script/global.lua");

function OnCreate(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		play:sendPacketToMsg(EC_TARGET, "["..play:getName().."]创建["..roomId.."]房间");
	end
	return 0;
end

function OnEnter(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]加入["..roomId.."]房间");
		
	end
	return 0;
end

function OnLeave(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]离开["..roomId.."]房间");
	end
	return 0;
end

function OnChangeMaster(roomId, oldUserId, newUserId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(newUserId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..roomId.."]房间更换房主为["..newUserId.."]");
	end
	return 0;
end