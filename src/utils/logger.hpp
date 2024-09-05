#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime> 


enum class LogType
{
	DEBUG, 
	INFO, 
	WARNING, 
	ERROR, 
	CRITICAL
};


namespace amanises
{
	class Logger
	{
	public:
		Logger(std::string& filepath);
		~Logger() { if (logFile) logFile.close(); };
		
		void log(LogType type, std::string& msg);

	private:
		std::ofstream logFile;

		std::string getLogTimestamp();

		std::string logTypeToStr(LogType type);
	};
}


