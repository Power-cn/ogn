#include "Shared.h"
#include "DateTime.h"
#include "log_outputer.h"
#include "log_file_outputer.h"

LogFileOutputer::LogFileOutputer(const std::string& path)
{
	m_path = path;
}

LogFileOutputer::~LogFileOutputer(void)
{
}

void LogFileOutputer::Outputer(const char* content, int color /* = 0 */, int level /* = logLevel_info */)
{
	FILE* file = NULL;
	fopen_s(&file, m_path.c_str(), "a");
	char temp[40];
	switch (level)
	{
	case logLevel_info:
		sprintf_s(temp, "INFO:  ");
		break;
	case logLevel_warn:
		sprintf_s(temp, "WARN:  ");
		break;
	case logLevel_error:
		sprintf_s(temp, "ERROR: ");
		break;
	case logLevel_debug:
		sprintf_s(temp, "DEBUG: ");
		break;
	}
	if (file)
	{
		fwrite(temp, strlen(temp), 1, file);

		std::string str("%Y-%m-%d %H:%M:%S ");
		DateTime::Now(str);
		if (str.length() > 0)
		{
			fwrite(str.c_str(), str.length(), 1, file);
		}

		fwrite(content, strlen(content), 1, file);
		fwrite((void*)"\r\n", 2, 1, file);
		fclose(file);
	}
}
