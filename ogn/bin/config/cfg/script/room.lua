
dofile("../config/cfg/script/global.lua");

function OnEnter(roomId, userId)
	local play = luaPlayer();
	play:sendPacketToMsg(EC_ROOM, "["..play:GetName().."]���뷿��");
	return 0;
end

function OnLeave(roomId, userId)
	return 0;
end

function OnChangeMaster(roomId, oldUserId, newUserId)
	return 0;
end