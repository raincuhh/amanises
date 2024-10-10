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
#include "lexer/lHelper.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "compilerSetup.hpp"

namespace amanises {
	class CompilerPipeline {
	public:
		CompilerPipeline(Logger* logger);
		void pipeline_init(int argc, char* argv[]);
	private:
		Logger* m_logger;

		void process_source_files(int argc, char* argv[], CompilerSetup* compiler_setup, std::vector<std::pair<std::string, std::vector<Token>>>& src_and_tok_lists, std::vector<const char*>& src_path_list);
		void tokenize_token_list(CompilerSetup* compiler_setup, std::vector<std::pair<std::string, std::vector<Token>>>& src_and_tok_lists, std::vector<const char*>& src_path_list);
	};
}

