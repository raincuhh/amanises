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
	class CompilerPipeline
	{
	public:
		CompilerPipeline(Logger* _m_logger);
		void pipeline_init(char* argv[]);

	private:
		Logger* m_logger;
		std::string read_file(const std::string& file_path);
		bool verify_opening_file_path(const char* file_path);
	};
}

