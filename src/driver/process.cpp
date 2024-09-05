#include "process.hpp"



amanises::Process::Process(char* argv[])
{
	init(argv);
}

void amanises::Process::init(char* argv[])
{
	std::string filepath = argv[1];
	if (!verifyFilepath(filepath))
	{
		std::cerr << "Error: File path verification failed." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string content;
	{
		//std::fstream input(argv[1], std::ios::in); // release
		std::fstream input(filepath, std::ios::in);  // debug
		if (!input)
		{
			std::cerr << "Error: Unable to open file " << filepath << std::endl;
		}

		std::stringstream contentStream;
		contentStream << input.rdbuf();
		content = contentStream.str();
	}

	std::unique_ptr<LexicalAnalyzer> lexer = std::make_unique<LexicalAnalyzer>(std::move(content));
	if (!lexer->init())
	{
		std::cerr << "Error: Lexer initialization failed." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<Token> tokens = lexer->getTokenList();
	lexer->printTokens(tokens);
}

bool amanises::Process::verifyFilepath(std::string& filepath)
{
	std::fstream file(filepath);
	if (!file.is_open())
	{
		return false;
	}
	return true;
}

