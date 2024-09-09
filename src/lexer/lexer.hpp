#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <assert.h>

#include "token.hpp"
#include "../logger.hpp"

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
			LEX_OPERATOR,
			LEX_PUNCTUATION,
			LEX_IDENTIFIER,
			LEX_LITERAL,
			LEX_PREPROC,
			LEX_COMMENTS,
			LEX_WHITE_SPACE,
			LEX_ERROR
		};

		explicit Lexer(std::string _content, size_t _contentLen, Logger* _logger);
		bool tokenize_content();

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
		Token get_next_token(std::string_view content, size_t& idx, lex_states& lex_state, std::string& tok_buf, std::vector<Token>& tok_list);
		token_kind determine_tok_kind(std::string& tok_buf);
		token_kind determine_literal_tok_kind(std::string& tok_buf);
		void clear_token_buffer(std::string& tok_buf) { tok_buf.clear(); };

		std::vector<std::string> split_to_chunk_buffers(const std::string& content, size_t max_chunk_size);
		
		std::string trim_white_space(std::string& content);
		std::string trim_word(const std::string& str);

		std::string token_kind_to_str(const token_kind type);
		std::string token_to_str(Token* token);

		void init_token_map();

		//checks for base cases
		inline bool is_chunk_buf_boundary_char(char c);
		inline bool is_space(char c);
		inline bool is_preproc(char c);
		inline bool is_alpha(char c);
		inline bool is_alpha_num(char c);
		bool is_operator(const std::string_view& content, size_t& idx);
		inline bool peek_ahead(const std::string_view& content, size_t& idx, char to_check);
		bool is_punctuator(char c);
		bool is_identifier(char c);
		bool is_potential_identifier_start(char c);
		bool is_potential_identifier_char(char c);
		bool is_digit(char c);

		void accumulate_preproc_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_operator_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_punctuation_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_identifier_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_literal_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void handle_comments(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void handle_white_space(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
	};
}