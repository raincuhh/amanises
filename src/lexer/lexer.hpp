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
#include "lHelper.hpp"
#include "../logger.hpp"

//context for the semantic expression checking later on

namespace amanises {
	class Lexer {
	public:

		enum class lex_states {
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

		explicit Lexer(Logger* logger);
		std::vector<Token> tokenize_source_file(std::string source);

	private:
		Logger* m_logger;
		std::unordered_map<std::string, token_kind> m_tok_map;
		size_t m_cur_line;
		size_t m_cur_col;

		void tokenize(std::string_view content, std::vector<Token>& tok_list);
		Token get_next_token(std::string_view content, size_t& idx, lex_states& lex_state, std::string& tok_buf, std::vector<Token>& tok_list);
		
		token_kind determine_tok_kind(std::string& tok_buf);
		token_kind determine_literal_tok_kind(std::string& tok_buf);

		void clear_token_buffer(std::string& tok_buf) { tok_buf.clear(); };

		std::vector<std::string> split_to_chunk_buffers(const std::string& content, size_t max_chunk_size);

		void init_token_map();
		void update_line_col(const std::string_view& content, size_t& idx);

		void accumulate_preproc_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_operator_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_punctuation_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_identifier_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void accumulate_literal_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		
		void handle_comments(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
		void handle_white_space(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list);
	};
}