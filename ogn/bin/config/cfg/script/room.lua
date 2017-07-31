dofile("../config/cfg/script/global.lua");

function OnCreate(roomId, userId)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		play:sendPacketToMsg(EC_TARGET, "["..play:getName().."]����["..roomId.."]����");
	end
	return 0;
end
function OnClose(roomId)

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

function OnChangeState(roomId, userId, oldState, state)
	local sWorld = luaWorld();
	local play = sWorld:getPlayerToUserId(userId);
	if play then
		local str = "";
		if state == RPS_None then
			str = "ȡ��׼��";
		end
		
		if state == RPS_Ready then
			str = "׼��";	
		end
		if state == RPS_Game then
			str = "��ʼ��Ϸ";	
		end
		
		if state == RPS_Observed then
			str = "�۵���";	
		end
		play:sendPacketToMsg(EC_ROOM, "����ID["..roomId.."]���ID["..userId.."]"..str);
	end
	return 0;
end