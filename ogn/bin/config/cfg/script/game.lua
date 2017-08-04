
dofile("../config/cfg/script/global.lua");

function OnStart(gameId)
	local sGame = luaGame();
	local agame = sGame:FindGameModle(gameId);
	local roomId = agame:GetRoomId();
	if agame then
		local num = agame:GetGameEntCount();
		for i=1,num do
			local gameEnt = agame:GetGameEnt(i - 1);
			if gameEnt then
				local play = luaFindPlrByUserId(gameEnt.userId);
				if play then
					play:sendPacketToMsg(EC_SYSTEM, "["..roomId.."]房间游戏开始！");
				end
			end
					
		end

	end
	return 0;
end

function OnClose(gameId)
	local sGame = luaGame();
	local agame = sGame:FindGameModle(gameId);
	local roomId = agame:GetRoomId();
	if agame then
		local num = agame:GetGameEntCount();
		for i=1,num do
			local gameEnt = agame:GetGameEnt(i - 1);
			if gameEnt then
				local play = luaFindPlrByUserId(gameEnt.userId);
				if play then
					play:sendPacketToMsg(EC_SYSTEM, "["..roomId.."]房间游戏结束！");
				end
			end
					
		end

	end
	return 0;
end

function OnEnter(gameId, userId)
	
	return 0;
end

function OnLeave(gameId, userId)

	return 0;
end
