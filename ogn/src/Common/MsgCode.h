#pragma once

enum MsgCode
{
	MC_None = 0,
	MC_LoginOk,
	MC_LoginTheWrongAccountOrPassword,
	MC_LoginTheNumberInTheRemote,
	MC_LoginYouHaveLanded,
	MC_InviterHaveTeam,
	MC_JoinerTeamNotExist,
	MC_HaveTeam,
	MC_TeamNotExist,
	MC_AgreeAddTeamNotLeader, // ͬ������������Ĳ��Ƕӳ�;
	MC_TeamError,

	MC_RoomFull,					// 
};

enum NetResult
{
	NResultFail,				// ʧ��
	NResultSuccess,				// �ɹ�
};