#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <direct.h>
#include <string>

#include "../utils/tokenTypes.hpp"
#include "../frontend/lexer.hpp"

using amanises::Lexer;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Incorrect usage. Correct usage..." << std::endl;
		std::cerr << argv[0] << " <source.ama>" << std::endl;
		return EXIT_FAILURE;
	}
	
	
	// getting cwd 
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

	// checking if file exists
	//std::ifstream file(argv[1]);
	std::ifstream file("../../../tests/unit/testTokenizer.ama");

	if (!file.is_open())
	{
		//std::cerr << "Could not open file: " << argv[1] << std::endl;
		return EXIT_FAILURE;
	}

	std::string contents;
	{
		std::stringstream contentsStream;
		//std::fstream input(argv[1], std::ios::in); //add this in when not debug
		std::fstream input("../../../tests/unit/testTokenizer.ama", std::ios::in); // remove when debug
		contentsStream << input.rdbuf();
		contents = contentsStream.str();
	}

	// lexing and tokenizing contents
	std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>();

	std::vector<Token> tokens = lexer->tokenize(contents);


	std::cout << "Amanises compilation finished." << std::endl;
	return EXIT_SUCCESS;
}