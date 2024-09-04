#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <direct.h>
#include <string>

#include "../utils/tokenTypes.hpp"
#include "../frontend/lexer.hpp"
#include "../error/error.hpp"

namespace amanises
{
	class Process
	{
	public:
		using Lexer = amanises::Lexer;

		Process(char* argv[]);
		void init(char* argv[]);

		bool verifyFilepath(std::string& filepath);

	private:

	};
}

