#pragma once


enum GridType
{
	GT_Prop,	 // ����;
	GT_Goods,	 // ��Ʒ;
};

class BackpackGrid
{
public:
	int8		type;
	int8		subType;
	uint32		itemId;
};

class Backpack
{
public:

protected:
	std::vector<BackpackGrid*> mListBackpackGrid;
};