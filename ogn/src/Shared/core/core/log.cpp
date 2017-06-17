#include "Shared.hpp"

log::log():
m_console(NULL)
{
	m_console = new LogConsoleOutpter;
	m_win = new LogWinOutputer;
	std::string strTimer = "";
	DateTime::Now(strTimer);
	m_file = new LogFileOutputer(strTimer + ".log");
	m_log.append(m_console);
	m_log.append(m_win);
	m_log.append(m_file);
}

log::~log()
{
	SAFE_DELETE(m_console);
	SAFE_DELETE(m_win);
	SAFE_DELETE(m_file);
}


