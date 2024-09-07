#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime> 

enum class log_type
{
	DEBUG, 
	INFO, 
	WARNING, 
	ERROR, 
	CRITICAL,
	EXCEPTION
};


namespace amanises
{
	class Logger
	{
	public:
		Logger(std::string& filepath);
		~Logger() { if (log_file) log_file.close(); };
		
		void log(log_type type, std::string msg);
		bool clear_logs();

	private:
		std::ofstream log_file;
		std::string mLogFilepath;

		std::string get_log_time_stamp();

		std::string log_type_to_str(log_type type);
	};
}


