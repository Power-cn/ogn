function OnCreate()
	local play = luaPlayer();
	play:sendPacketToMsg(EC_SYSTEM, "["..play:getName().."]������ɫ�ɹ�");
	return 0;
end

function OnDispose()
	return 0;
end

function OnEnter()
	local tab = GetTestTable();
	
	for key, value in pairs(t) do  
		print(value);   
	end  
	return 0;
end

function OnLeave()
	return 0;
end