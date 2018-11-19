
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
					play:sendPacketToMsg(EC_SYSTEM, "["..roomId.."]������Ϸ��ʼ��");
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
					play:sendPacketToMsg(EC_SYSTEM, "["..roomId.."]������Ϸ������");
				end
			end
					
		end

	end
	return 0;
end

function OnEnter(gameId, userId)
	local aGame = luaGoldenFlower(gameId);
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	local count = aGameEnt:GetCardCount();
	local str = "";
	local sCfgMgr = luaCfg();
		
	for i = 0, count - 1, 1 do
		local card = aGameEnt:GetCard(i);
		local cardinfo = sCfgMgr:getCardJson(card);
		str = str.."["..cardinfo.Name.."]";
	end	
	play:sendPacketToMsg(EC_SYSTEM, str);
	return 0;
end

function OnLeave(gameId, userId)

	return 0;
end

function OnSeeCard(userId)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]".."����");
	return 0;
end

-- ��
function OnChipinReq(userId, gold)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]".."��ע"..gold.."���");
	return 0;
end

--��
function OnCallReq(userId, gold)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]".."��ע"..gold.."���");
	return 0;
end

function OnCompareReq(userId, tarUserId, result)
	local aGame = luaGameComponent();
	local aGameEnt = aGame:FindGameEnt(userId);
	local play = luaFindPlrByUserId(userId);
	local tarPlay = luaFindPlrByUserId(tarUserId);
	local strret = result == 1 and "Ӯ" or "��";
	play:sendPacketToMsg(EC_ROOM, "["..play:getName().."]����".."["..tarPlay:getName().."]["..strret.."]");
	return 0;
end