#include "lexer.hpp"

amanises::Lexer::Lexer(std::string _content, size_t _contentLen, Logger* _logger) :
	logger(_logger),
	content(std::move(_content)),
	contentLen(_contentLen),
	cursor(),
	line(),
	col(),
	fullTokenList(),
	tokMap()
{
	initTokMap();
}

bool amanises::Lexer::lexContent()
{
	// preprocess
	
	// splits up source into chunks for tokenization
	// TODO: might eventually make the BUFFER_SIZE be dynamically set between 8kb and 16kb
	const size_t bufSize = 8192;
	std::vector<std::string> contentBuffers = splitToBuffers(content, bufSize);

	for (const std::string& buf : contentBuffers)
	{
		// tokenize to a buffer token list
		std::vector<Token> bufTokenList;
		tokenize(buf, bufTokenList);

		// reserve and move the buffer token list to the back of full token list 
		fullTokenList.reserve(fullTokenList.size() + bufTokenList.size());
		std::move(bufTokenList.begin(), bufTokenList.end(), std::back_inserter(fullTokenList));
	}

	// check if the full token list is still empty after tokenization, (should atleast have a EOF token is why)
	if (fullTokenList.empty())
	{
		logger->log(LogType::ERROR, "Full token list empty after tokenization.");
		return false;
	}

	return true;
}

void amanises::Lexer::debugPrintTokens(std::vector<Token>& tokens)
{
	if (tokens.empty()) return;

	for (const auto& token : tokens)
	{
		std::cout << "token (type: " << getTokenTypeStr(token.type) << ", value: ";

		if (token.val)
		{
			std::cout << token.val.value();
		}
		else
		{
			std::cout << "nulloptr";
		}
		std::cout << ")" << std::endl;
	}
}

void amanises::Lexer::tokenize(std::string_view content, std::vector<Token>& tokenList)
{
	LexStates state = LexStates::LEX_ST_INITIAL;
	std::string buf;

	std::cout << content << std::endl;

	for (size_t i = 0; i < content.length();)
	{
		char c = content[i];
		//std::cout << c << std::endl;

		i++;

		switch (state)
		{
		case LexStates::LEX_ST_INITIAL: 
			/*
			TODO:
			check for whitespace
			check for preproc
			check for comments
			check for keyword
			check for data types
			check for operator
			check for punctuator
			check for identifiers
			check for literals
			*/
			
			break;
		case LexStates::LEX_ST_PREPROC: 
			
			break;
		case LexStates::LEX_ST_COMMENTS:
			
			break;
		case LexStates::LEX_ST_KEYWORD:

			break;
		case LexStates::LEX_ST_DATA_TYPES:

			break;
		case LexStates::LEX_ST_OPERATOR:

			break;
		case LexStates::LEX_ST_PUNCTUATION:

			break;

		case LexStates::LEX_ST_IDENTIFIER:

			break;
		case LexStates::LEX_ST_LITERALS:

			break;
		default:

			break;
		}
	}

	tokenList.push_back(Token{ .type = TokenType::TOK_EOF });
}

std::string amanises::Lexer::trimWhite(std::string& content)
{
	content.reserve(content.size());

	content.erase(std::remove_if(content.begin(), content.end(), [](char c) {
		return std::isspace(c);
		}), content.end());
	return content;
}

std::vector<std::string> amanises::Lexer::splitToBuffers(const std::string& content, size_t maxChunkSize)
{
	// TODO: edgecases to solve:
	// tracking syntax boundaries. as in open and closed delimiters.
	// tokenization marks kinda

	std::vector<std::string> chunks;
	size_t contentSize = content.size();
	size_t start = 0;

	while (start < contentSize) {

		size_t end = std::min(start + maxChunkSize, contentSize);

		// ensure we only split on a boundary character
		while (end < contentSize && !Utils::isBoundaryCharacter(content[end])) {
			++end;
		}

		// get the chunk
		std::string chunk = content.substr(start, end - start);
		chunks.push_back(chunk);

		// debug, printing out chunk size
		std::cout << "chunkSize: " << contentSize << std::endl;

		start = end;
	}
	return chunks;
}

std::string amanises::Lexer::getTokenTypeStr(const TokenType type)
{
	switch (type)
	{
	// reserved keywords
	case TokenType::TOK_IF:            return "IF";
	case TokenType::TOK_ELSE:          return "ELSE";
	case TokenType::TOK_FOR:           return "FOR";
	case TokenType::TOK_WHILE:         return "WHILE";
	case TokenType::TOK_RETURN:        return "RETURN";
	case TokenType::TOK_BREAK:         return "BREAK";
	case TokenType::TOK_CONTINUE:      return "CONTINUE";
	case TokenType::TOK_SWITCH:        return "SWITCH";
	case TokenType::TOK_CASE:          return "CASE";
	case TokenType::TOK_DEFAULT:       return "DEFAULT";

	case TokenType::TOK_CLASS:         return "CLASS";
	case TokenType::TOK_PRIVATE:       return "PRIVATE";
	case TokenType::TOK_PROTECTED:     return "PROTECTED";
	case TokenType::TOK_PUBLIC:        return "PUBLIC";
	case TokenType::TOK_STATIC:        return "STATIC";

	case TokenType::TOK_NEW:           return "NEW";
	case TokenType::TOK_DELETE:        return "DELETE";

	// data types
	case TokenType::TOK_INT:           return "INT";
	case TokenType::TOK_FLOAT:         return "FLOAT";
	case TokenType::TOK_DOUBLE:        return "DOUBLE";
	case TokenType::TOK_CHAR:          return "CHAR";
	case TokenType::TOK_STRING:        return "STRING";
	case TokenType::TOK_BOOL:          return "BOOL";
	case TokenType::TOK_VOID:          return "VOID";

	// operators
	case TokenType::TOK_PLUS:          return "PLUS";
	case TokenType::TOK_MINUS:         return "MINUS";
	case TokenType::TOK_MULTIPLY:      return "MULTIPLY";
	case TokenType::TOK_DIVIDE:        return "DIVIDE";
	case TokenType::TOK_ASSIGN:        return "ASSIGN";
	case TokenType::TOK_EQUAL:         return "EQUAL";
	case TokenType::TOK_NOT_EQUAL:     return "NOT_EQUAL";
	case TokenType::TOK_LESS_THAN:     return "LESS_THAN";
	case TokenType::TOK_GREATER_THAN:  return "GREATER_THAN";
	case TokenType::TOK_LESS_EQUAL:    return "LESS_EQUAL";
	case TokenType::TOK_GREATER_EQUAL: return "GREATER_EQUAL";
	case TokenType::TOK_AND:           return "AND";
	case TokenType::TOK_OR:            return "OR";
	case TokenType::TOK_NOT:           return "NOT";
	case TokenType::TOK_INCREMENT:     return "INCREMENT";
	case TokenType::TOK_DECREMENT:     return "DECREMENT";

	// punctuation
	case TokenType::TOK_SEMICOLON:     return "SEMICOLON";
	case TokenType::TOK_COLON:         return "COLON";
	case TokenType::TOK_DOT:           return "DOT";
	case TokenType::TOK_COMMA:         return "COMMA";
	case TokenType::TOK_OPEN_PAR:      return "OPEN_PAR";
	case TokenType::TOK_CLOSE_PAR:     return "CLOSE_PAR";
	case TokenType::TOK_OPEN_BRACE:    return "OPEN_BRACE";
	case TokenType::TOK_CLOSE_BRACE:   return "CLOSE_BRACE";
	case TokenType::TOK_OPEN_BRACKET:  return "OPEN_BRACKET";
	case TokenType::TOK_CLOSE_BRACKET: return "CLOSE_BRACKET";

	// identifier
	case TokenType::TOK_IDENTIFIER:    return "IDENTIFIER";

	// literals
	case TokenType::TOK_INTEGER_LIT:   return "INTEGER_LIT";
	case TokenType::TOK_FLOAT_LIT:     return "FLOAT_LIT";
	case TokenType::TOK_CHAR_LIT:      return "CHAR_LIT";
	case TokenType::TOK_STRING_LIT:    return "STRING_LIT";
	case TokenType::TOK_BOOLEAN_LIT:   return "BOOLEAN_LIT";
	case TokenType::TOK_NULL_LIT:      return "NULL_LIT";

	// comments
	case TokenType::TOK_LINE_COMMENT:  return "LINE_COMMENT";
	case TokenType::TOK_BLOCK_COMMENT: return "BLOCK_COMMENT";

	// function related
	case TokenType::TOK_FUNCTION:      return "FUNCTION";
	case TokenType::TOK_METHOD:        return "METHOD";
	case TokenType::TOK_RETURN_TYPE:   return "RETURN_TYPE";

	// start and end of file
	case TokenType::TOK_EOF:           return "_EOF";
	case TokenType::TOK_SOF:           return "_SOF";

	// preprocessors
	case TokenType::TOK_PRAGMA:        return "PRAGMA";
	case TokenType::TOK_INCLUDE:       return "INCLUDE";

	// error handling
	case TokenType::TOK_ERROR:         return "ERROR";
	default:                           return "UNDEFINED";
	}
}

void amanises::Lexer::initTokMap()
{
	tokMap = {
		// punctuation
		{ ";", TokenType::TOK_SEMICOLON },
		{ ":", TokenType::TOK_COLON },
		{ ".", TokenType::TOK_DOT },
		{ ",", TokenType::TOK_COMMA },
		{ "(", TokenType::TOK_OPEN_PAR },
		{ ")", TokenType::TOK_CLOSE_PAR },
		{ "{", TokenType::TOK_OPEN_BRACE },
		{ "}", TokenType::TOK_CLOSE_BRACE },
		{ "[", TokenType::TOK_OPEN_BRACKET },
		{ "]", TokenType::TOK_CLOSE_BRACKET },

		// operators
		{ "+", TokenType::TOK_PLUS },
		{ "-", TokenType::TOK_MINUS },
		{ "*", TokenType::TOK_MULTIPLY },
		{ "/", TokenType::TOK_DIVIDE },
		{ "=", TokenType::TOK_ASSIGN },
		{ "==", TokenType::TOK_EQUAL },
		{ "!=", TokenType::TOK_NOT_EQUAL },
		{ "<", TokenType::TOK_LESS_THAN },
		{ ">", TokenType::TOK_GREATER_THAN },
		{ "<=", TokenType::TOK_LESS_EQUAL },
		{ ">=", TokenType::TOK_GREATER_EQUAL },
		{ "&&", TokenType::TOK_AND },
		{ "||", TokenType::TOK_OR },
		{ "!", TokenType::TOK_NOT },
		{ "++", TokenType::TOK_INCREMENT },
		{ "--", TokenType::TOK_DECREMENT },

		// keywords
		{ "if", TokenType::TOK_IF },
		{ "else", TokenType::TOK_ELSE },
		{ "for", TokenType::TOK_FOR },
		{ "while", TokenType::TOK_WHILE },
		{ "return", TokenType::TOK_RETURN },
		{ "break", TokenType::TOK_BREAK },
		{ "continue", TokenType::TOK_CONTINUE },
		{ "switch", TokenType::TOK_SWITCH },
		{ "case", TokenType::TOK_CASE },
		{ "default", TokenType::TOK_DEFAULT },

		{ "class", TokenType::TOK_CLASS },
		{ "private", TokenType::TOK_PRIVATE },
		{ "protected", TokenType::TOK_PROTECTED },
		{ "public", TokenType::TOK_PUBLIC },
		{ "static", TokenType::TOK_STATIC },

		{ "new", TokenType::TOK_NEW },
		{ "delete", TokenType::TOK_DELETE },

		// data Types
		{ "int", TokenType::TOK_INT },
		{ "float", TokenType::TOK_FLOAT },
		{ "double", TokenType::TOK_DOUBLE },
		{ "char", TokenType::TOK_CHAR },
		{ "string", TokenType::TOK_STRING },
		{ "bool", TokenType::TOK_BOOL },
		{ "void", TokenType::TOK_VOID },

		// preprocessors
		{ "#pragma", TokenType::TOK_PRAGMA },
		{ "#include", TokenType::TOK_INCLUDE },

		// literals  are handled by the lexer when it comes to that point

		// errors    are handled by the lexer when it comes to that point

	};
}



/*
std::vector<Token> amanises::LexicalAnalyzer::tokenize(const std::string& source)
{
	std::vector<Token> tokens;
	std::string buf;



	tokens.push_back({ .type = TokenType::TOK_SOF });

	for (size_t idx = 0; idx < source.length();)
	{
		char c = source.at(idx);

		if (std::isalpha(c))
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
				tokens.push_back({.type = TokenType::TOK_RETURN});
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

			tokens.push_back({ .type = TokenType::TOK_INTEGER_LIT, .value = buf });
			//buf.clear();
			continue;
		}

		else if (c == ';')
		{
			tokens.push_back({ .type = TokenType::TOK_SEMICOLON });
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
	tokens.push_back({ .type = TokenType::TOK_EOF });

	return tokens;
}
*/