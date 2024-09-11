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
#include "error.hpp"
#include "logger.hpp"


namespace amanises
{
	class CompilerSetup 
	{
	public:
		CompilerSetup(Logger* logger, Lexer* lexer);

		bool process_file_for_lexer(const char* src_path, std::string& src_name, std::vector<Token>& src_tok_list);
		//bool process_tokens_for_parser();

	private:
		Logger* m_logger;
		Lexer* m_lexer;

		std::string read_file(const std::string& file_path);
		bool verify_opening_file_path(const char* file_path);
		std::string get_file_name(const std::string& file_path);
	};
}


