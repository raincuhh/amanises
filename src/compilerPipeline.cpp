#include "compilerPipeline.hpp"

amanises::CompilerPipeline::CompilerPipeline(Logger* logger) :
	m_logger(logger)
{
}

void amanises::CompilerPipeline::pipeline_init(int argc, char* argv[])
{
	std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(m_logger);
	std::unique_ptr<CompilerSetup> compilerSetup = std::make_unique<CompilerSetup>(m_logger, lexer.get());

	std::vector<std::pair<std::string, std::vector<Token>>> src_and_tok_lists;


	// initial processing of source files, and the lexing of said files
	for (size_t i = 1; i < argc; i++)
	{
		const char* src_path = argv[i];
		std::string src_name;
		std::vector<Token> src_tok_list;
		
		if (!compilerSetup->process_file_for_lexer(src_path, src_name, src_tok_list))
		{
			exit(EXIT_FAILURE);
		}

		src_and_tok_lists.emplace_back(src_name, src_tok_list);
	}

	// debug printing 
	for (auto& pair : src_and_tok_lists)
	{
		std::vector<Token> tokens = pair.second;
		std::string src_name = pair.first;

		//std::cout << src_name << std::endl;
		//lexer->print_tokens_verbose(tokens); 
		//std::cout << "total tok from src: `" << tokens.size() << "`" << std::endl;
	}

	// starting parsing/abstract syntax tree/ast process
	//m_logger->log(log_type::INFO, std::string("Ast parsing started."));
	//m_logger->log(log_type::INFO, std::string("Ast parsing finished."));
}