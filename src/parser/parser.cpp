#include "parser.hpp"

amanises::Parser::Parser(Logger* logger, std::vector<Token> src_tok_list) :
	m_logger(logger),
	m_cur_src_tok_list(std::move(src_tok_list))
{
}

void amanises::Parser::parse_token_list(std::vector<Token> src_tok_list)
{
}
