
dofile("../config/cfg/script/global.lua");

CT_Block = 1;
CT_Club = 2;
CT_Heart = 3;
CT_Spade = 4;
CT_Joker = 5;

JT_Small = 1;
JT_Big = 2;

-- 0等于 1大 2小 --
function CompareCard(card1, card2)
	if (card1 > card2) then
		return 1;
	end
	
	if (card1 < card2) then
		return 2;
	end
	return 0;
end

function ComparePlrCard(userId1, userId2)

	return 0;
end