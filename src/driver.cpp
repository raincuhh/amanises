#include "driver.hpp"

amanises::Driver::Driver()
{
}

bool amanises::Driver::driver_init(int argc, char* argv[])
{
	std::string log_path = "C:/dev/repos/amanises/logs/log.txt";
	std::unique_ptr<Logger> logger = std::make_unique<Logger>(log_path);

	if (validate_usage_arguments(argc, argv, logger.get()) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	if (check_working_directory(logger.get()) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	return compiler_pipeline_init(argc, argv, logger.get());
}

bool amanises::Driver::validate_usage_arguments(int argc, char* argv[], Logger* m_logger)
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

bool amanises::Driver::check_working_directory(Logger* logger)
{
    try
    {
        std::filesystem::path cwd = std::filesystem::current_path();
        logger->log(log_type::INFO, "Working directory: " + cwd.string());
        return EXIT_SUCCESS;
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        logger->log(log_type::ERROR, "Failed getting working directory: " + std::string(e.what()));
        return EXIT_FAILURE;
    }
}

bool amanises::Driver::compiler_pipeline_init(int argc, char* argv[], Logger* m_logger)
{
	m_logger->log(log_type::INFO, std::string("Amanises compilation started."));

	try
	{
		std::unique_ptr<CompilerPipeline> compiler_pipeline = std::make_unique<CompilerPipeline>(m_logger);
		compiler_pipeline->pipeline_init(argc, argv);
	}
	catch (const std::exception& e)
	{
		m_logger->log(log_type::EXCEPTION, std::string(e.what()));
		return EXIT_FAILURE;
	}

	m_logger->log(log_type::INFO, std::string("Amanises compilation finished."));
	//delete m_logger;
	return EXIT_SUCCESS;
}
