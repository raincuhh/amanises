#include <iostream>
#include <fstream>
#include <memory>
#include <direct.h>
#include <string>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Incorrect usage. Correct usage..." << std::endl;
		std::cerr << "./amanises <input.ama>" << std::endl;
		return EXIT_FAILURE;
	}
		
	char buffer[256];
	if (!_getcwd(buffer, sizeof(buffer)))
	{
		std::cerr << "Error getting current working directory" << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "Working directory: " << buffer << std::endl;
	}

	const char *fileName = argv[1];
	std::ifstream file(fileName);

	if (!file.is_open())
	{
		std::cerr << "Could not open file: " << fileName << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::cout << "opened file: " << fileName << std::endl;
	}

	std::string fileContent;
	std::string line;

	while (std::getline(file, line))
	{
		fileContent += line + "\n";
	}
	std::cout << fileContent << std::endl;

	file.close();
	return EXIT_SUCCESS;
}