#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

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

		enum class lex_states
		{
			LEX_ST_INITIAL = 0,
			LEX_ST_PREPROC,
			LEX_ST_COMMENTS,
			LEX_ST_KEYWORD,
			LEX_ST_DATA_TYPES,
			LEX_ST_OPERATOR,
			LEX_ST_PUNCTUATION,
			LEX_ST_IDENTIFIER,
			LEX_ST_LITERALS,
			LEX_ST_ERROR
			// might add states for stuff like preprocessor conditionals, etc.
		};

		explicit Lexer(std::string _content, size_t _contentLen, Logger* _logger);

		bool lex_content();

		void debug_print_tokens(std::vector<Token>& tokens);
		std::vector<Token> get_full_token_list() const { return std::move(full_tok_list); };
	private:
		Logger* m_logger;

		std::string m_content;
		//std::string_view mContentView;
		
		size_t m_content_len;
		size_t m_cursor;
		size_t m_line;
		size_t m_col;

		std::vector<Token> full_tok_list;

		void tokenize(std::string_view content, std::vector<Token>& tok_list);
		std::string trim_white_space(std::string& content);
		std::vector<std::string> split_to_buffers(const std::string& content, size_t max_chunk_size);
		std::string get_token_type_by_str(const token_type type);

		void init_u_tok_map();

		std::unordered_map<std::string, token_type> tokMap;

		//checks for different cases

	};
}