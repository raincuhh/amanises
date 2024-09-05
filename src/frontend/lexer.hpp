#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../utils/tokenTypes.hpp"
#include "../utils/helper.hpp"

namespace amanises
{
	class LexicalAnalyzer
	{
	public:
		using Helper = amanises::Helper;

		explicit LexicalAnalyzer(std::string source);
		bool init();

		std::vector<Token> tokenize(const std::string& source);
		std::vector<Token> getTokenList() const { return mTokenList; };
		void printTokens(std::vector<Token>& tokens);
		std::string getTokenTypeStr(const TokenType token);
	private:
		std::vector<Token> mTokenList;
		const std::string mSource;
	};
}