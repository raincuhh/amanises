#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <direct.h>
#include <string>
#include <string_view>

#include "lexer/token.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/ast.hpp"
#include "error.hpp"
#include "logger.hpp"


namespace amanises {
	class CompilerSetup {
	public:
		CompilerSetup(Logger* logger, Lexer* lexer, Parser* parser);

		bool process_file_for_lexer(const char* src_path, std::string& src_name, std::vector<Token>& src_tok_list);
		bool process_tok_list_for_parser(const char* src_path, const std::string& src_name, std::vector<Token>& src_tok_list);

	private:
		Logger* m_logger;
		Lexer* m_lexer;
		Parser* m_parser;


		// TODO: put these in another class eventually. they are generalized helpers
		std::string read_file(const std::string& file_path);
		bool verify_opening_file_path(const char* file_path);
		std::string get_file_name_with_suffix(const std::string& file_path);
		std::string get_file_name_without_suffix(const std::string& file_path);
	};
}