#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../utils/tokenTypes.hpp"
#include "../utils/helper.hpp"

namespace amanises
{
	class Lexer
	{
	public:
		using Helper = amanises::Helper;

		explicit Lexer(std::string source);
		bool init();

		std::vector<Token> tokenize(const std::string& source);
		std::vector<Token> getTokenList() const { return mTokenList; };
	private:
		std::vector<Token> mTokenList;
		const std::string mSource;
	};
}