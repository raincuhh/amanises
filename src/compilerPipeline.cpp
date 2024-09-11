#include "compilerPipeline.hpp"

amanises::CompilerPipeline::CompilerPipeline(Logger* _m_logger) :
	m_logger(_m_logger)
{
}

void amanises::CompilerPipeline::pipeline_init(int argc, char* argv[])
{
	std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(m_logger);
	std::vector<std::pair<std::string, std::vector<Token>>> sources_and_tok_lists;

	for (size_t i = 1; i < argc; i++)
	{
		const char* path = argv[i];
		m_logger->log(log_type::INFO, "Lexing process started for " + std::string(path));

		// verify
		if (!verify_opening_file_path(path))
		{
			m_logger->log(log_type::ERROR, std::string("File path verification failed for: ") + path);
			exit(EXIT_FAILURE);
		}

		// read file
		std::string source = read_file(path);
		if (source.empty())
		{
			m_logger->log(log_type::ERROR, std::string("Failed to read or empty file: ") + path);
			exit(EXIT_FAILURE);
		}

		// get file name
		std::string source_name = get_file_name(path);
		if (source_name.empty()) 
		{
			m_logger->log(log_type::ERROR, "Unable to determine file name for: " + std::string(path));
			exit(EXIT_FAILURE);
		}

		// tokenize file
		std::vector<Token> source_tok_list = lexer->tokenize_source_file(std::move(source));
		if (source_tok_list.empty()) {
			m_logger->log(log_type::ERROR, source_name + " has an empty token list after tokenization");
			exit(EXIT_FAILURE);
		}

		// storing file
		sources_and_tok_lists.emplace_back(source_name, source_tok_list);
	}

	m_logger->log(log_type::INFO, std::string("Lexing processing finished."));

	
	/*
	// get tokens
	std::vector<Token> tokens = lexer->get_full_token_list();

	lexer->print_tokens_non_verbose(tokens);
	std::cout << "total tok from src: `" << tokens.size() << "`" << std::endl;
	*/
	

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

std::string amanises::CompilerPipeline::get_file_name(const std::string& file_path)
{
	return file_path.substr(file_path.find_last_of("//\\") + 1);
}
