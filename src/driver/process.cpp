#include "process.hpp"

// TODO: future add compiling multiple source files, currently only does one.

amanises::Process::Process(char* argv[], Logger* logger) : 
	m_logger(logger)
{
	init(argv);
}

void amanises::Process::init(char* argv[])
{
	const char* filepath = argv[1]; // for now its just argv[1] but eventually it will go through all files inputted
	if (!verify_file_path(filepath))
	{
		m_logger->log(log_type::ERROR, std::string("File path verification failed for: ") + filepath);
		exit(EXIT_FAILURE);
	}

	std::string src;
	{
		std::fstream input(filepath, std::ios::in);
		if (!input)
		{
			std::cerr << "Error: Unable to open file " << filepath << std::endl;
		}

		std::stringstream c_stream;
		c_stream << input.rdbuf();
		src = c_stream.str();
	}

	// starting lexing process
	m_logger->log(log_type::INFO, std::string("Lexing processing started."));

	std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(
		std::move(src),
		src.length(),
		m_logger
	);
	src.clear();

	// tokenizing
	if (!lexer->process_content())
	{
		m_logger->log(log_type::ERROR, std::string("Failed lexing tokenizing."));
		exit(EXIT_FAILURE);
	}

	// get tokens
	std::vector<Token> tokens = lexer->get_full_token_list();
	lexer->debug_print_tokens(tokens);

	m_logger->log(log_type::INFO, std::string("Lexing processing finished."));


	// starting parsing/abstract syntax tree/ast process
	//m_logger->log(log_type::INFO, std::string("Ast parsing started."));
	//m_logger->log(log_type::INFO, std::string("Ast parsing finished."));

}

bool amanises::Process::verify_file_path(const char* filepath)
{
	std::fstream file(filepath);
	if (!file.is_open())
	{
		return false;
	}
	return true;
}

