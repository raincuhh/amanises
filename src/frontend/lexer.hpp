#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

#include "../utils/tokens.hpp"
#include "../utils/utils.hpp"
#include "../utils/logger.hpp"

namespace amanises
{
	class Lexer
	{
	public:
		using Utils = amanises::Utils;
		using Logger = amanises::Logger;

		explicit Lexer(std::string content, size_t contentLen, Logger* logger);

		bool processContent();

		void debugPrintTokens(std::vector<Token>& tokens);
		std::vector<Token> getFullTokenList() const { return std::move(mFullTokenList); };
	private:
		Logger* mLogger;

		std::string mContent;
		//std::string_view mContentView;
		
		size_t mContentLen;
		size_t cursor;
		size_t line;
		size_t col;

		std::vector<Token> mFullTokenList;

		std::vector<Token> tokenize(std::string_view content);
		std::string trimWhite(std::string& content);
		std::string getTokenTypeStr(const TokenType type);
		bool tokenValueIsNotEmpty(const Token& token);
	};
}