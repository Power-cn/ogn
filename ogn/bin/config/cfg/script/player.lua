dofile("../config/cfg/script/global.lua");

function OnCreate(userId)
	local play = luaPlayer();
	play:sendPacketToMsg(EC_SYSTEM, "["..play:getName().."]������ɫ�ɹ�");
	return 0;
end

function OnDispose(userId)
	return 0;
end

function OnEnter(userId)

	return 0;
end

function OnLeave(userId)
	return 0;
end