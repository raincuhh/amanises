#include <iostream>
#include <memory>
#include <string>

#include "process.hpp"
#include "../utils/logger.hpp"

using Process = amanises::Process;
using Logger = amanises::Logger;

static bool compile(char* argv[], Logger* logger)
{
	logger->log(LogType::INFO, std::string("Amanises compilation started."));

	try
	{
		std::unique_ptr<Process> process = std::make_unique<Process>(argv, logger);
	}
	catch (const std::exception& e)
	{
		logger->log(LogType::EXCEPTION, std::string(e.what()));
		return EXIT_FAILURE;
	}

	logger->log(LogType::INFO, std::string("Amanises compilation finished."));
	return EXIT_SUCCESS;
}

static bool checkForCWD(Logger* logger)
{
	char buf[256];
	if (!_getcwd(buf, sizeof(buf)))
	{
		logger->log(LogType::ERROR, std::string("Failed getting working directory"));
		exit(EXIT_FAILURE);
	}
	else
	{
		logger->log(LogType::INFO, std::string("Working directory ") + buf);
	}
}

static bool checkForCorrectUsage(int argc, char* argv[])
{
	// argc has to have atleast 2 
	if (argc < 2)
	{
		std::cerr << "Incorrect usage. Correct usage..." << std::endl;
		std::cerr << argv[0] << " <source.ama>" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[])
{
	// checking for invalid compiler usage
	checkForCorrectUsage(argc, argv);

	// starting logger
	std::string logPath = "../../../logs/log.txt";
	std::unique_ptr<Logger> logger = std::make_unique<Logger>(logPath);
	
	// checking current working directory
	checkForCWD(logger.get());

	// starting compilation process
	return compile(argv, logger.get());
}