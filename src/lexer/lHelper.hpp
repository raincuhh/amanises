#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "token.hpp"

namespace amanises {
	class LexerHelper {
	public:
		static inline std::string tok_kind_to_str(token_kind kind);

		static void print_tokens_verbose(std::vector<Token>& tok_list); 
		static void print_tokens_non_verbose(std::vector<Token>& tok_list); 

		static std::string token_to_str_verbose(Token* token);
		static std::string token_to_str_non_verbose(Token* token);

		static bool peek_ahead(const std::string_view& content, size_t& idx, char to_check);
		static bool is_chunk_buf_boundary_char(char c);
		static bool is_preproc_start(char c);

		static bool is_space(char c);
		static bool is_alpha(char c);
		static bool is_alpha_num(char c);
		static bool is_digit(char c);

		static bool is_operator(const std::string_view& content, size_t& idx);
		static bool is_punctuator(char c);
		static bool is_identifier_start(char c);
		static bool is_identifier(char c);
		static bool is_literal_start(char c);
		static bool is_literal(char c);

		static std::string trim_str(const std::string& str);
		static std::string trim_white_space_str(std::string& str);

	private:

	};
}



