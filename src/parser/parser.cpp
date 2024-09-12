#include "parser.hpp"

amanises::Parser::Parser(Logger* logger) :
	m_logger(logger) {
}

void amanises::Parser::parse_token_list(std::vector<Token>&& src_tok_list) {

	for (size_t idx = 0; idx < src_tok_list.size() && src_tok_list[idx].kind != token_kind::TOK_EOF; idx++) {
		Token token = src_tok_list[idx];
		//std::cout << "token" << std::endl;
	}
}
