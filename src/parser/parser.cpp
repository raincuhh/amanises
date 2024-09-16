#include "parser.hpp"

amanises::Parser::Parser(Logger* logger) :
	m_logger(logger) {
}

// TODO: probably gnna make this return a ast for the next semantic analysis part
void amanises::Parser::parse_token_list(std::vector<Token>&& src_tok_list) {

	for (size_t idx = 0; idx < src_tok_list.size() && src_tok_list[idx].kind != token_kind::TOK_EOF; idx++) {
		Token token = src_tok_list[idx];
		std::cout << LexerHelper::tok_kind_to_str(token.kind) << std::endl;
	}
}
