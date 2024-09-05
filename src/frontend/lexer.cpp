#include "lexer.hpp"

amanises::LexicalAnalyzer::LexicalAnalyzer(std::string source) :
	mSource(std::move(source))
{
}

bool amanises::LexicalAnalyzer::init()
{
	mTokenList = tokenize(std::move(mSource));
	if (mTokenList.empty())
	{
		std::cerr << "Error: tokenlist empty after tokenization" << std::endl;
		return false;
	}
	return true;
}

std::vector<Token> amanises::LexicalAnalyzer::tokenize(const std::string& source)
{
	std::vector<Token> tokens;
	std::string buf;

	tokens.push_back({ .type = TokenType::_SOF });

	for (size_t idx = 0; idx < source.length();)
	{
		char c = source.at(idx);

		if (std::isalpha(c)/*Utils::isAlpha(c)*/)
		{
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

void amanises::LexicalAnalyzer::printTokens(std::vector<Token>& tokens)
{
	for (const auto& token : tokens)
	{
		std::cout << "token (type: " << getTokenTypeStr(token.type) << ", value: ";

		if (token.value.has_value()) {
			std::cout << token.value.value();  
		}
		else {
			std::cout << "nulloptr";
		}

		std::cout << ")" << std::endl;
	}
}

std::string amanises::LexicalAnalyzer::getTokenTypeStr(const TokenType type)
{
	switch (type)
	{
	// reserved keywords
	case TokenType::IF:            return "IF";
	case TokenType::ELSE:          return "ELSE";
	case TokenType::FOR:           return "FOR";
	case TokenType::WHILE:         return "WHILE";
	case TokenType::RETURN:        return "RETURN";
	case TokenType::BREAK:         return "BREAK";
	case TokenType::CONTINUE:      return "CONTINUE";
	case TokenType::SWITCH:        return "SWITCH";
	case TokenType::CASE:          return "CASE";
	case TokenType::DEFAULT:       return "DEFAULT";

	case TokenType::CLASS:         return "CLASS";
	case TokenType::PRIVATE:       return "PRIVATE";
	case TokenType::PROTECTED:     return "PROTECTED";
	case TokenType::PUBLIC:        return "PUBLIC";
	case TokenType::STATIC:        return "STATIC";

	case TokenType::NEW:           return "NEW";
	case TokenType::DELETE:        return "DELETE";

	// data types
	case TokenType::INT:           return "INT";
	case TokenType::FLOAT:         return "FLOAT";
	case TokenType::DOUBLE:        return "DOUBLE";
	case TokenType::CHAR:          return "CHAR";
	case TokenType::STRING:        return "STRING";
	case TokenType::BOOL:          return "BOOL";
	case TokenType::VOID:          return "VOID";

	// operators
	case TokenType::PLUS:          return "PLUS";
	case TokenType::MINUS:         return "MINUS";
	case TokenType::MULTIPLY:      return "MULTIPLY";
	case TokenType::DIVIDE:        return "DIVIDE";
	case TokenType::ASSIGN:        return "ASSIGN";
	case TokenType::EQUAL:         return "EQUAL";
	case TokenType::NOT_EQUAL:     return "NOT_EQUAL";
	case TokenType::LESS_THAN:     return "LESS_THAN";
	case TokenType::GREATER_THAN:  return "GREATER_THAN";
	case TokenType::LESS_EQUAL:    return "LESS_EQUAL";
	case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
	case TokenType::AND:           return "AND";
	case TokenType::OR:            return "OR";
	case TokenType::NOT:           return "NOT";
	case TokenType::INCREMENT:     return "INCREMENT";
	case TokenType::DECREMENT:     return "DECREMENT";

	// punctuation
	case TokenType::SEMICOLON:     return "SEMICOLON";
	case TokenType::COLON:         return "COLON";
	case TokenType::DOT:           return "DOT";
	case TokenType::COMMA:         return "COMMA";
	case TokenType::OPEN_PAR:      return "OPEN_PAR";
	case TokenType::CLOSE_PAR:     return "CLOSE_PAR";
	case TokenType::OPEN_BRACE:    return "OPEN_BRACE";
	case TokenType::CLOSE_BRACE:   return "CLOSE_BRACE";
	case TokenType::OPEN_BRACKET:  return "OPEN_BRACKET";
	case TokenType::CLOSE_BRACKET: return "CLOSE_BRACKET";

	// identifier and literals
	case TokenType::IDENTIFIER:    return "IDENTIFIER";
	case TokenType::INTEGER_LIT:   return "INTEGER_LIT";
	case TokenType::FLOAT_LIT:     return "FLOAT_LIT";
	case TokenType::CHAR_LIT:      return "CHAR_LIT";
	case TokenType::STRING_LIT:    return "STRING_LIT";
	case TokenType::BOOLEAN_LIT:   return "BOOLEAN_LIT";
	case TokenType::NULL_LIT:      return "NULL_LIT";

	// comments
	case TokenType::LINE_COMMENT:  return "LINE_COMMENT";
	case TokenType::BLOCK_COMMENT: return "BLOCK_COMMENT";

	// function related
	case TokenType::FUNCTION:      return "FUNCTION";
	case TokenType::METHOD:        return "METHOD";
	case TokenType::RETURN_TYPE:   return "RETURN_TYPE";

	// start and end of file
	case TokenType::_EOF:          return "_EOF";
	case TokenType::_SOF:          return "_SOF";

	// preprocessors
	case TokenType::PRAGMA:        return "PRAGMA";
	case TokenType::IMPORT:        return "IMPORT";

	// error handling
	case TokenType::ERROR:         return "ERROR";

	default:
		return "UNDEFINED";
	}
}
