#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../utils/tokenTypes.hpp"

namespace amanises
{
	class Lexer
	{
	public:
		void init();
		std::vector<Token> tokenize(const std::string& str);

		std::vector<Token> getTokenList();
	private:
		std::vector<Token> tokenList;
	};

}