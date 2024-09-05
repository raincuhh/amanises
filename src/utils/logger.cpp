#include "logger.hpp"

amanises::Logger::Logger(std::string& filepath) :
	mLogFilepath(filepath)
{
	logFile.open(filepath, std::ios::app);
	if (!logFile.is_open())
	{
		log(LogType::ERROR, std::string("Failed opening logfile") + filepath);
	}
}

void amanises::Logger::log(LogType type, std::string msg)
{
	std::string timestamp = getLogTimestamp();
	std::ostringstream entry;

	entry 
		<< "[" << timestamp << "] "
		<< logTypeToStr(type) << ": "
		<< msg;

	std::cout << entry.str() << std::endl;

	if (logFile.is_open())
	{
		entry
			<< std::endl;
		logFile << entry.str();
		logFile.flush();
	}
}

bool amanises::Logger::clearLogs()
{
	if (remove(mLogFilepath.c_str()))
	{
		return true;
	}
	return false;
}

std::string amanises::Logger::getLogTimestamp()
{
	time_t now = time(0);
	tm* localTime = localtime(&now);

	char timestamp[20];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localTime);

	return static_cast<std::string>(timestamp);
}

std::string amanises::Logger::logTypeToStr(LogType type)
{
	switch (type)
	{
	case LogType::DEBUG:	return "DEBUG";
	case LogType::INFO:		return "INFO";
	case LogType::WARNING:  return "WARNING";
	case LogType::ERROR:    return "ERROR";
	case LogType::CRITICAL: return "CRITICAL";
	default:				return "UNKNOWN";
	}
}
