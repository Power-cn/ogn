dofile("../config/cfg/script/global.lua");

function OnCreate(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		play:sendPacketToMsg(EC_TARGET, "["..play:getName().."]����["..roomId.."]����");
	end
	return 0;
end

function OnEnter(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]����["..roomId.."]����");
		
	end
	return 0;
end

function OnLeave(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]�뿪["..roomId.."]����");
	end
	return 0;
end

function OnChangeMaster(roomId, oldUserId, newUserId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(newUserId);
	if play then
		play:sendPacketToMsg(EC_ROOM, "["..roomId.."]�����������Ϊ["..newUserId.."]");
	end
	return 0;
end