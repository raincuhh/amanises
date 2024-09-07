#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "../utils/token.hpp"
#include "../utils/logger.hpp"

//context for the semantic expression checking later on

namespace amanises
{
	class Lexer
	{
	public:
		using Logger = amanises::Logger;

		enum class lex_states
		{
			LEX_INITIAL = 0,
			LEX_PREPROC,
			LEX_COMMENTS,
			LEX_KEYWORD,
			LEX_DATA_TYPE,
			LEX_OPERATOR,
			LEX_PUNCTUATION,
			LEX_IDENTIFIER,
			LEX_LITERALS,
			LEX_ERROR,
			LEX_WHITESPACE
			// might add states for stuff like preprocessor conditionals, etc.
		};

		explicit Lexer(std::string _content, size_t _contentLen, Logger* _logger);
		bool process_content();

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
		std::unordered_map<std::string, token_kind> tokMap;

		void tokenize(std::string_view content, std::vector<Token>& tok_list);
		std::vector<std::string> split_to_buffers(const std::string& content, size_t max_chunk_size);
		
		std::string trim_white_space(std::string& content);
		std::string get_token_kind_str(const token_kind type);

		void init_u_tok_map();

		//checks for base cases
		bool is_buf_boundary_char(char _c);
		bool is_space(char _c);
		bool is_preproc(char _c);
		inline bool is_alpha(char _c);
		inline bool is_alpha_num(char _c);
		bool is_operator(const std::string_view& content, size_t& idx);
		bool peek_ahead_by_char(const std::string_view& content, size_t& idx, char to_check);
		bool is_punctuator(char _c);
		bool is_identifier(char _c);
		bool is_digit(char _c);


		void handle_state_preproc(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void handle_state_whitespace(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
	};
}