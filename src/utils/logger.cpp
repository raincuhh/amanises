#include "logger.hpp"

amanises::Logger::Logger(std::string& filepath) :
	mLogFilepath(filepath)
{
	log_file.open(filepath, std::ios::app);
	if (!log_file.is_open())
	{
		log(log_type::ERROR, std::string("Failed opening logfile") + filepath);
	}
}

void amanises::Logger::log(log_type type, std::string msg)
{
	std::string timestamp = get_log_time_stamp();
	std::ostringstream entry;

	// making the entry msg entry format
	entry 
		<< "[" << timestamp << "] "
		<< log_type_to_str(type) << ": "
		<< msg;

	std::cout << entry.str() << std::endl;


	// flushing the file after log
	if (log_file.is_open())
	{
		entry
			<< std::endl;
		log_file << entry.str();
		log_file.flush();
	}
}

bool amanises::Logger::clear_logs()
{
	if (remove(mLogFilepath.c_str()))
	{
		return true;
	}
	return false;
}

std::string amanises::Logger::get_log_time_stamp()
{
	time_t now = time(0);
	tm* local_time = localtime(&now);

	char timestamp[20];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);

	return static_cast<std::string>(timestamp);
}

std::string amanises::Logger::log_type_to_str(log_type type)
{
	switch (type)
	{
	case log_type::DEBUG:	return "DEBUG";
	case log_type::INFO:		return "INFO";
	case log_type::WARNING:  return "WARNING";
	case log_type::ERROR:    return "ERROR";
	case log_type::CRITICAL: return "CRITICAL";
	default:				return "UNKNOWN";
	}
}
