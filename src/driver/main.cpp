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

	std::unique_ptr<Logger> logger = std::make_unique<Logger>();
	
	// getting current working directory
	char buf[256];
	if (!_getcwd(buf, sizeof(buf)))
	{
		std::cerr << "Error getting working directory" << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "Working directory: " << buf << std::endl;
	}

	// starting compilation process
	std::cout << "Amanises compilation starting." << std::endl;

	// ensure argv[1] is valid
	std::unique_ptr<Process> process;
	try
	{
		process = std::make_unique<Process>(argv);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Amanises compilation finished." << std::endl;
	return EXIT_SUCCESS;
}