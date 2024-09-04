#include "lexer.hpp"

void amanises::Lexer::init()
{
}

std::vector<Token> amanises::Lexer::tokenize(const std::string& str)
{
	std::vector<Token> tokens;
	std::string buf;

	for (int idx = 0; idx < str.length(); idx++)
	{
		char c = str.at(idx);

		if (std::isalpha(c))
		{
			buf.push_back(c);
			idx++;
			while (std::isalnum(str.at(idx)))
			{
				buf.push_back(str.at(idx));
				idx++;
			}
			idx--;

			if (buf == "return")
			{ 
				tokens.push_back({.type = TokenType::RETURN});
				buf.clear();
				continue;
			}
			else
			{
				std::cerr << "you messed up" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		else if (std::isdigit(c))
		{
			buf.push_back(c);
			idx++;

			while (std::isdigit(str.at(idx)))
			{
				buf.push_back(str.at(idx));
				idx++;
			}
			idx--;

			tokens.push_back({ .type = TokenType::INTEGER_LIT, .value = buf });
			buf.clear();
			continue;
		}

		else if (c == ';')
		{
			tokens.push_back({ .type = TokenType::SEMICOLON });
		}

		else if (std::isspace(c))
		{
			continue;
		}

		else
		{
			std::cerr << "you might have messed up major" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	return tokens;
}

std::vector<Token> amanises::Lexer::getTokenList()
{
	return tokenList;
}
