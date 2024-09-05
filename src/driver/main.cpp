#include <iostream>
#include <memory>
#include <string>

#include "process.hpp"
#include "../utils/logger.hpp"

int main(int argc, char* argv[])
{
	using Process = amanises::Process;
	using Logger = amanises::Logger;

	if (argc < 2)
	{
		std::cerr << "Incorrect usage. Correct usage..." << std::endl;
		std::cerr << argv[0] << " <source.ama>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string logPath = "../../../logs/log.txt";
	std::unique_ptr<Logger> logger = std::make_unique<Logger>(logPath);
	
	// getting current working directory
	char buf[256];
	if (!_getcwd(buf, sizeof(buf)))
	{
		logger->log(LogType::ERROR, std::string("Failed getting working directory"));
		return EXIT_FAILURE;
	}
	else
	{
		logger->log(LogType::INFO, std::string("Working directory ") + buf);
	}

	// starting compilation process
	logger->log(LogType::INFO, std::string("Amanises compilation started."));

	try
	{
		std::unique_ptr<Process> process = std::make_unique<Process>(argv, logger.get());
	}
	catch (const std::exception& e)
	{
		logger->log(LogType::EXCEPTION, std::string(e.what()));
		return EXIT_FAILURE;
	}

	logger->log(LogType::INFO, std::string("Amanises compilation finished."));
	return EXIT_SUCCESS;
}