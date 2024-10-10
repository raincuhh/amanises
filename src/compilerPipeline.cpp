#include "compilerPipeline.hpp"

amanises::CompilerPipeline::CompilerPipeline(Logger* logger) :
	m_logger(logger) {
}

void amanises::CompilerPipeline::pipeline_init(int argc, char* argv[]) {
	std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(m_logger);
	std::unique_ptr<Parser> parser = std::make_unique<Parser>(m_logger);
	std::unique_ptr<CompilerSetup> compiler_setup = std::make_unique<CompilerSetup>(m_logger, lexer.get(), parser.get());

	std::vector<std::pair<std::string, std::vector<Token>>> src_and_tok_lists;
	std::vector<const char*> src_path_list;

	// initial processing of source files, and the tokenization of said source files
	process_source_files(argc, argv, compiler_setup.get(), src_and_tok_lists, src_path_list);

	// debug printing lexing result
	LexerHelper::debug_print_src_and_tok_list(src_and_tok_lists);
	
	// starting the processing of src file parsing into ast
	tokenize_token_list(compiler_setup.get(), src_and_tok_lists, src_path_list);
}

void amanises::CompilerPipeline::process_source_files(int argc, char* argv[], CompilerSetup* compiler_setup, std::vector<std::pair<std::string, std::vector<Token>>>& src_and_tok_lists, std::vector<const char*>& src_path_list)
{
	for (size_t i = 1; i < argc; i++) {
		const char* src_path = argv[i];
		std::string src_name;
		std::vector<Token> src_tok_list;

		if (!compiler_setup->process_file_for_lexer(src_path, src_name, src_tok_list)) {
			exit(EXIT_FAILURE);
		}

		src_and_tok_lists.emplace_back(src_name, src_tok_list);
		src_path_list.emplace_back(src_path);
	}
}

void amanises::CompilerPipeline::tokenize_token_list(CompilerSetup* compiler_setup, std::vector<std::pair<std::string, std::vector<Token>>>& src_and_tok_lists, std::vector<const char*>& src_path_list)
{
	for (size_t i = 0; i < src_and_tok_lists.size(); i++) {

		// passing src_path around for reference for logging stuff.
		const char* src_path = src_path_list[i];
		std::string src_name = src_and_tok_lists[i].first;
		std::vector<Token> src_tok_list = src_and_tok_lists[i].second;

		if (!compiler_setup->process_tok_list_for_parser(src_path, src_name, src_tok_list)) {
			exit(EXIT_FAILURE);
		}
	}
}
