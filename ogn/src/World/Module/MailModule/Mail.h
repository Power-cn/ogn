#pragma once

enum MailType
{
	MailT_None,
	MailT_Target,
	MailT_Total,
};

class MailAttach
{
public:
protected:
private:
};

class Mail
{
public:
	Mail();
	~Mail();
protected:
	MailType					mMailType = MailT_None;
	uint32						mFromUserId = 0;
	uint32						mTargetUserId = 0;
	std::string					mTitle;
	std::string					mContent;
};