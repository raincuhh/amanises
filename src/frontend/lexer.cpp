#include "lexer.hpp"

amanises::Lexer::Lexer(std::string source) :
	mSource(std::move(source))
{
}

bool amanises::Lexer::init()
{
	mTokenList = tokenize(std::move(mSource));
	if (mTokenList.empty())
	{
		std::cerr << "Error: tokenlist empty after tokenization" << std::endl;
		return false;
	}
	return true;
}

std::vector<Token> amanises::Lexer::tokenize(const std::string& source)
{
	std::vector<Token> tokens;
	std::string buf;

	for (size_t idx = 0; idx < source.length();)
	{
		char c = source.at(idx);

		if (idx == 0)
		{
			tokens.push_back({ .type = TokenType::_SOF });
		}

		if (std::isalpha(c))
		{
			//buf.clear();
			//buf.push_back(c);
			//idx++;
			//while (std::isalnum(source.at(idx)))
			//{
			//	buf.push_back(source.at(idx));
			//	idx++;
			//}
			//idx--;

			buf.clear();
			do
			{
				buf.push_back(c);
				++idx;
				if (idx < source.length())
				{
					c = source.at(idx);
				}

			} while (idx < source.length() && std::isalnum(c));

			if (buf == "return")
			{ 
				tokens.push_back({.type = TokenType::RETURN});
				buf.clear();
			}
			else
			{
				std::cerr << "you messed up, buf: " << buf << std::endl;
				exit(EXIT_FAILURE);
			}
			continue;
		}

		else if (std::isdigit(c))
		{
			//buf.push_back(c);
			//idx++;
//
			//while (std::isdigit(source.at(idx)))
			//{
			//	buf.push_back(source.at(idx));
			//	idx++;
			//}
			//idx--;

			buf.clear();
			do
			{
				buf.push_back(c);
				++idx;
				if (idx < source.length())
				{
					c = source.at(idx);
				}
			} while (idx < source.length() && std::isdigit(c));

			tokens.push_back({ .type = TokenType::INTEGER_LIT, .value = buf });
			//buf.clear();
			continue;
		}

		else if (c == ';')
		{
			tokens.push_back({ .type = TokenType::SEMICOLON });
			idx++;
		}

		else if (std::isspace(c))
		{
			idx++;
		}

		else
		{
			std::cerr << "you might have messed up major" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	tokens.push_back({ .type = TokenType::_EOF });

	return tokens;
}