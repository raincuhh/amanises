#include "process.hpp"

// TODO: future add compiling multiple source files, currently only does one.

amanises::Process::Process(char* argv[], Logger* l) : 
	mLogger(l)
{
	init(argv);
}

void amanises::Process::init(char* argv[])
{
	const char* filepath = argv[1]; // for now its just argv[1] but eventually it will go through all files inputted
	if (!verifyFilepath(filepath))
	{
		mLogger->log(LogType::ERROR, std::string("File path verification failed for: ") + filepath);
		exit(EXIT_FAILURE);
	}

	std::string content;
	{
		std::fstream input(filepath, std::ios::in);
		if (!input)
		{
			std::cerr << "Error: Unable to open file " << filepath << std::endl;
		}

		std::stringstream contentStream;
		contentStream << input.rdbuf();
		content = contentStream.str();
	}

	// starting lexing process
	mLogger->log(LogType::INFO, std::string("Lexing tokenization started."));

	std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(std::move(content), content.length(), mLogger);
	content.clear();

	if (!lexer->lexContent())
	{
		mLogger->log(LogType::ERROR, std::string("Failed lexing processing."));
		exit(EXIT_FAILURE);
	}

	// get tokens after lexer has finished tokenizing
	std::vector<Token> tokens = lexer->getFullTokenList();
	lexer->debugPrintTokens(tokens);

	mLogger->log(LogType::INFO, std::string("Lexing tokenization finished."));


	// starting parsing/abstract syntax tree/ast process
	mLogger->log(LogType::INFO, std::string("Ast parsing started."));
	mLogger->log(LogType::INFO, std::string("Ast parsing finished."));

}

bool amanises::Process::verifyFilepath(const char* filepath)
{
	std::fstream file(filepath);
	if (!file.is_open())
	{
		return false;
	}
	return true;
}

