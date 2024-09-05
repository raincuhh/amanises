#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <direct.h>
#include <string>
#include <string_view>

#include "../utils/tokens.hpp"
#include "../frontend/lexer.hpp"
#include "../error/error.hpp"
#include "../utils/logger.hpp"

namespace amanises
{
	class Process
	{
	public:
		using Lexer = amanises::Lexer;
		using Logger = amanises::Logger;

		Process(char* argv[], Logger* l);

	private:
		Logger* mLogger;

		void init(char* argv[]);

		bool verifyFilepath(const char* filepath);
	};
}

