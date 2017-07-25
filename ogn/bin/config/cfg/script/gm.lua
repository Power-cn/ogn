
local gmManager = {};

local function RegGm(obj)
	if (obj.name ~= nil) then
		gmManager[obj.name] = obj;
	end
end

function GmCmd(s1, s2, s3, s4)
	local player = luaPlayer();
	gmManager[s1].fun(player, s2, s3, s4);
	return 0;
end

RegGm(
{
name = "test";
fun=function (player, s2, s3, s4)
	local entity = luaEntity();
	local propertyHelper = luaPropertyHelper();
	if propertyHelper == nil then
	end
	propertyHelper:setMaxHp(entity, 1);
end
});