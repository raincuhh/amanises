#include <iostream>
#include <memory>
#include <string>

#include "process.hpp"
#include "../utils/logger.hpp"

using Process = amanises::Process;
using Logger = amanises::Logger;

static bool amanises_compile(char* content[], Logger* m_logger)
{
	m_logger->log(log_type::INFO, std::string("Amanises compilation started."));

	try
	{
		std::unique_ptr<Process> process = std::make_unique<Process>(content, m_logger);
	}
	catch (const std::exception& e)
	{
		m_logger->log(log_type::EXCEPTION, std::string(e.what()));
		return EXIT_FAILURE;
	}

	m_logger->log(log_type::INFO, std::string("Amanises compilation finished."));
	return EXIT_SUCCESS;
}

static bool check_for_cwd(Logger* m_logger)
{
	char buf[256];
	if (!_getcwd(buf, sizeof(buf)))
	{
		m_logger->log(log_type::ERROR, std::string("Failed getting working directory"));
		return EXIT_FAILURE;
	}
	else
	{
		m_logger->log(log_type::INFO, std::string("Working directory ") + buf);
		return EXIT_SUCCESS;
	}
}

static bool check_for_correct_usage(int argc, char* argv[], Logger* m_logger)
{
	if (argc < 2)
	{
		m_logger->log(log_type::CRITICAL, std::string("Incorrect usage. Correct usage...") + argv[0] + std::string(" <source.ama>"));
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}
}

int main(int argc, char* argv[])
{
	std::string log_path = "../../../logs/log.txt";
	std::unique_ptr<Logger> logger = std::make_unique<Logger>(log_path);

	if (check_for_correct_usage(argc, argv, logger.get()) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (check_for_cwd(logger.get()) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	return amanises_compile(argv, logger.get());
}