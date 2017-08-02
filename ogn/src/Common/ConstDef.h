#pragma once

static const char* sScriptPlayer = "player";
static const char* sScriptRoom = "room";
static const char* sScriptGame = "game";
static const char* sScriptTeam = "team";
static const char* sScriptCard = "card";

static const char* sKey = "ogn";
static const int32 sKeyXor = 1314;


enum CardType
{
	CT_None = 0,
	CT_Block,			// ��
	CT_Club,			// ÷
	CT_Heart,			// ��
	CT_Spade,			// ��
	CT_Joker,			// ��
};

enum JokerType
{
	JT_None,
	JT_Small,			// С
	JT_Big,				// ��
};