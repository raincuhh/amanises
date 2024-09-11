#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <memory>

#include "../../src/logger.hpp"
#include "../../src/lexer/token.hpp"

namespace amanises
{
	class Parser
	{
	public:
		Parser(Logger* logger, std::vector<Token> src_tok_list);
		
		void parse_token_list(std::vector<Token> src_tok_list);

	private:
		Logger* m_logger; 
		std::vector<Token> m_cur_src_tok_list;
	};

}

