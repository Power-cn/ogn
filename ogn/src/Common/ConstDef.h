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
	CT_Block,			// ·½
	CT_Club,			// Ã·
	CT_Heart,			// ºì
	CT_Spade,			// ºÚ
	CT_Joker,			// ¹í
};

enum JokerType
{
	JT_None,
	JT_Small,			// Ð¡
	JT_Big,				// ´ó
};