#include "parser.hpp"

amanises::Parser::Parser(Logger* logger) :
	m_logger(logger)
{
}

void amanises::Parser::parse_token_list(std::vector<Token>&& src_tok_list)
{
	std::cout << src_tok_list.size() << std::endl;

	for (size_t idx = 1; idx < src_tok_list.size() && src_tok_list[idx].kind != token_kind::TOK_EOF; idx++)
	{

		std::cout << "token" << std::endl;
	}
}
