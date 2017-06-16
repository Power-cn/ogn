
dofile( "config/script/global.lua" );

function onEnterTeam(plrName)
	print(plrName);
	local sWorld = luaWorld();
	print(plrName);
	if sWorld==nil then
		print("sWorld==nil");
		return 0;
	end
	sWorld:getPlayerToUserId(1);
	print(plrName);
	sWorld:sendPacketToMsg(EC_WORLD, "asdfsadfsdfsdf", nil);
	print(plrName);
	return 0;
end

function onLeaveTeam(plrName)
	print(plrName);
	return 0;
end

function onChangeLeader(oldName, newName)
	return 0;
end