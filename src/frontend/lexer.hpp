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

		explicit Lexer(std::string _content, size_t _contentLen, Logger* _logger);

		bool processContent();

		void debugPrintTokens(std::vector<Token>& tokens);
		std::vector<Token> getFullTokenList() const { return std::move(fullTokenList); };
	private:
		Logger* logger;

		std::string content;
		//std::string_view mContentView;
		
		size_t contentLen;
		size_t cursor;
		size_t line;
		size_t col;

		std::vector<Token> fullTokenList;

		void tokenize(std::string_view content, std::vector<Token>& tokenList);
		std::string trimWhite(std::string& content);
		std::vector<std::string> splitToBuffers(const std::string& content, size_t maxChunkSize);
		std::string getTokenTypeStr(const TokenType type);
		bool tokenValueIsNotEmpty(const Token& token);
		bool isBoundaryCharacter(char c);
	};
}