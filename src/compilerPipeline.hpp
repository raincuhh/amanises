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
#include "lexer/lexHelper.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "compilerSetup.hpp"

namespace amanises
{
	class CompilerPipeline
	{
	public:

		CompilerPipeline(Logger* logger);
		void pipeline_init(int argc ,char* argv[]);

	private:
		Logger* m_logger;

	};
}

