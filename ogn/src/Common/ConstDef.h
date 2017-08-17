#pragma once

static const char* sScriptPlayer = "player";
static const char* sScriptRoom = "room";
static const char* sScriptGame = "game";
static const char* sScriptTeam = "team";
static const char* sScriptCard = "card";

static const char* sKey = "ogn";

static const uint32 sSpeakTotalTime = 30;
static const uint32 sMaxRoleCount = 3;


enum SndTarget : char
{
	Snd_Ssn,
	Snd_Plr,
	Snd_Sck,
};

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

enum ThreeCardType
{
	TCT_None,
	TCT_Danzhang,				// ��
	TCT_Duizi,					// ��
	TCT_Shunzi,					// ˳
	TCT_Tonghua,				// ͬ
	TCT_Tonghuashun,			// ͬ˳
	TCT_Baozi,					// ��
};

enum GameOperateType
{
	GOperT_None = 0,
	GOperT_See,						// ��
	GOperT_Chipin,					// ��
	GOperT_CallChipin,				// ��
	GOperT_VS,						// ��
};

enum VSType
{
	VST_None,
	VST_Win,
	VST_Lose,
};