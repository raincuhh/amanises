#include "compilerPipeline.hpp"

amanises::CompilerPipeline::CompilerPipeline(Logger* _m_logger) :
	m_logger(_m_logger)
{
}

void amanises::CompilerPipeline::pipeline_init(char* argv[])
{
	// verify
	const char* file_path = argv[1];
	if (!verify_opening_file_path(file_path))
	{
		m_logger->log(log_type::ERROR, std::string("File path verification failed for: ") + file_path);
		exit(EXIT_FAILURE);
	}

	std::string src = read_file(file_path);

	// starting lexing process
	m_logger->log(log_type::INFO, std::string("Lexing processing started."));

	std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(
		std::move(src),
		src.length(),
		m_logger
	);
	src.clear();

	// tokenizing
	if (!lexer->tokenize_content())
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

std::string amanises::CompilerPipeline::read_file(const std::string& file_path)
{
	std::fstream input(file_path, std::ios::in);
	if (!input)
	{
		std::cerr << "Error: Unable to open file " << file_path << std::endl;
	}

	std::stringstream c_stream;
	c_stream << input.rdbuf();
	input.close();

	return c_stream.str();
}

// TODO: add this somewhere else, idk
bool amanises::CompilerPipeline::verify_opening_file_path(const char* file_path)
{
	std::fstream file(file_path);
	if (!file.is_open())
	{
		return false;
	}
	return true;
}
