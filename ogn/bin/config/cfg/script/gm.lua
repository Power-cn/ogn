
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
fun=function (player, s1, s2, s3)
	local entity = luaEntity();
	local propertyHelper = luaPropertyHelper();
	if propertyHelper == nil then
	end
	propertyHelper:setMaxHp(entity, 1);
end
});

RegGm(
{
name = "croom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoCreateRoom(player);
end
});

RegGm(
{
name = "enroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoEnterRoom(player, tonumber(s1));
end
});

RegGm(
{
name = "leroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoLeaveRoom(player, tonumber(s1));
end
});


RegGm(
{
name = "cmroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoChangeRoomMaster(player, tonumber(s1), tonumber(s2));
end
});


RegGm(
{
name = "lstroom";
fun=function (player, s1, s2, s3)
	local luaRoom = luaRoom();
	luaRoom:DoRoomList(player, tonumber(s1), tonumber(s2));
end
});