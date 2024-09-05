#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <direct.h>
#include <string>

#include "../utils/tokenTypes.hpp"
#include "../frontend/lexicalAnalyzer.hpp"
#include "../error/error.hpp"

namespace amanises
{
	class Process
	{
	public:
		using LexicalAnalyzer = amanises::LexicalAnalyzer;

		Process(char* argv[]);
		void init(char* argv[]);

		bool verifyFilepath(std::string& filepath);

	private:

	};
}

