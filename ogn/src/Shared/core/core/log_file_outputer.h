#pragma once


class LogFileOutputer : public LogOutputer
{
public:
	LogFileOutputer(const std::string& path);
	~LogFileOutputer(void);
public:
	virtual void Outputer(const char* content, int color, int level);
protected:
	std::string						m_path;
};

