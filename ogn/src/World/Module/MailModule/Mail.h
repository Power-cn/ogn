#pragma once

enum MailType
{
	MT_None,
	MT_Target,
	MT_Total,
};

class Mail
{
public:
	Mail();
	~Mail();
protected:
	MailType					mMailType = MT_None;
	uint32						mFromUserId = 0;
	uint32						mTargetUserId = 0;
	std::string					mTitle;
	std::string					mContent;
};