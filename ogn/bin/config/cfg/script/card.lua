
dofile("../config/cfg/script/global.lua");

CT_Block = 1;
CT_Club = 2;
CT_Heart = 3;
CT_Spade = 4;
CT_Joker = 5;

JT_Small = 1;
JT_Big = 2;



function SortCards(cards)
	if cards[2].Number > cards[1].Number then
		local swap = cards[1];
		cards[1] = cards[2];
		cards[2] = swap;
	end
	
	if cards[2].Number > cards[3].Number then
		local swap = cards[2];
		cards[2] = cards[3];
		cards[3] = swap;
	end
	
	if cards[2].Number > cards[1].Number then
			local swap = cards[1];
			cards[1] = cards[2];
			cards[2] = swap;
	end
	return cards;
end

function IsBaozi(card1, card2, card3)
	if card1.Number == card2.Number and card1.Number == card3.Number then
		return 1;
	end
	return 0;
end

function IsTonghuashun(card1, card2, card3)
	if IsTonghua(card1, card2, card3) == 0 then
		return 0;
	end
	
	return IsShunzi(card1, card2, card3);
end

function IsTonghua(card1, card2, card3)
	if card1.Color == card2.Color and card1.Color == card3.Color then
		return 1;
	end
	return 0;
end

function IsShunzi(card1, card2, card3)
	local cards = {card1, card2, card3};
	cards = SortCards(cards);
	if (cards[1].Number + 1 == cards[1].Number and
		cards[2].Number + 1 == cards[3].Number)then
		return 1;
	end

	if (cards[3].Number - cards[2].Number == 12 and
		cards[2].Number + 1 == cards[3].Number)then
		return 1;
	end
	return 0;
end

function IsDuizi(card1, card2, card3)
	if (card1.Number == card2.Number or 
		card1.Number == card3.Number or 
		card2.Number == card3.Number) then
		return 1;
	end
	return 0;
end

function IsDanzhang(card1, card2, card3)
	return 1;
end

function CheckThreeCardType(card1, card2, card3)

	if (IsBaozi(card1, card2, card3)) then
		return TCT_Baozi;
	end
	
	if (IsTonghuashun(card1, card2, card3)) then
		return TCT_Tonghuashun;
	end
	
	if (IsTonghua(card1, card2, card3)) then
		return TCT_Tonghua;
	end
	
	if (IsShunzi(card1, card2, card3)) then
		return TCT_Shunzi;
	end
	
	if (IsDuizi(card1, card2, card3)) then
		return TCT_Duizi;
	end
	
	if (IsDanzhang(card1, card2, card3)) then
		return TCT_Danzhang;
	end
	return 0;
end

function CheckDanzhang(cards1, cards2)
	SortCards(cards1);
	SortCards(cards2);
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];

	if u1card3.Number > u2card3.Number then
		return 1;
	end

	if u1card3.Number < u2card3.Number then
		return 2;
	end
	
	if u1card2.Number > u2card2.Number then
		return 1;
	end
	
	if u1card2.Number < u2card2.Number then
		return 2;
	end
	
	if u1card1.Number > u2card1.Number then
		return 1;
	end

	if u1card1.Number < u2card1.Number then
		return 2;
	end
			
	if u1card3.Color > u2card3.Color then
		return 1;
	end

	return 2;
end

function CheckDuizi(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];	
	return 0;
end

function CheckShunzi(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];	
	return 0;
end

function CheckTonghua(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];	
	return 0;
end

function CheckTonghuashun(cards1, cards2)
	local u1card1 = cards1[1];
	local u1card2 = cards1[2];
	local u1card3 = cards1[3];

	local u2card1 = cards2[1];
	local u2card2 = cards2[2];
	local u2card3 = cards2[3];	
	return 0;
end

function CheckBaozi(cards1, cards2)
	local u1card1 = cards1[1];
	local u2card3 = cards2[3];	
	if (u1card1.Number > u2card1.Nu)then
		return 1;
	end
	return 2;
end

function CompareCards(type, cards1, cards2)
	if  type == TCT_Danzhang then
		return CheckDanzhang(cards1, cards2);
	end

	if  type == TCT_Duizi then
		return CheckDuizi(cards1, cards2);
	end

	if  type == TCT_Shunzi then
		return CheckShunzi(cards1, cards2);
	end

	if  type == TCT_Tonghua then
		return CheckTonghua(cards1, cards2);
	end

	if  type == TCT_Tonghuashun then
		return CheckTonghuashun(cards1, cards2);
	end

	if  type == TCT_Baozi then
		return CheckBaozi(cards1, cards2);
	end
end

function ComparePlrCard(userId1, userId2)
	local aGame = luaGame();
	local user1card1 = aGame:GetPlrCard(userId1, 0);
	local user1card2 = aGame:GetPlrCard(userId1, 1);
	local user1card3 = aGame:GetPlrCard(userId1, 2);
	
	local user2card1 = aGame:GetPlrCard(userId2, 0);
	local user2card2 = aGame:GetPlrCard(userId2, 1);
	local user2card3 = aGame:GetPlrCard(userId2, 2);

	local sCfgMgr = luaCfg();
	local u1card1 = sCfgMgr:GetCardJson(user1card1);
	local u1card2 = sCfgMgr:GetCardJson(user1card2);
	local u1card3 = sCfgMgr:GetCardJson(user1card3);
	
	local u2card1 = sCfgMgr:GetCardJson(user2card1);
	local u2card2 = sCfgMgr:GetCardJson(user2card2);
	local u2card3 = sCfgMgr:GetCardJson(user2card3);
		
	local u1cardtype = CheckThreeCardType(u1card1, u1card2, u1card3);
	local u2cardtype = CheckThreeCardType(u2card1, u2card2, u2card3);

	if (u1cardtype > u2cardtype) then
		return 1;
	end
	
	if (u1cardtype < u2cardtype) then
		return 2;
	end
	local cards1 = {user1card1, user1card2, user1card3};
	local cards2 = {user2card1, user2card2, user2card3};
	return CompareCards(u1cardtype, cards1, cards2);
end