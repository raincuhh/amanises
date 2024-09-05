#include "lexer.hpp"

amanises::Lexer::Lexer(std::string content, size_t contentLen, Logger* logger) :
	mLogger(logger),
	mContent(std::move(content)),
	mContentLen(contentLen),
	cursor(),
	line(),
	col(),
	mFullTokenList()
{
}

bool amanises::Lexer::processContent()
{
	// preprocess the content

	// TODO: make whitespace exception rules, stuff like <int<typeshi> >, and some other 
	// cases, might just move this to the tokenizer if it becomes annoying
	// this is highly experimental, might change in the future probably.
	
	//mContent = trimWhite(mContent);
	//std::cout << mContent << std::endl;

	// TODO: make a more advanced content splitter.
	// splits up source into chunks for tokenization
	std::vector<std::string> contentChunks;
	contentChunks = splitIntoChunks(mContent, 8000);

	std::vector<Token> bufTokList;


	return true;
}

void amanises::Lexer::debugPrintTokens(std::vector<Token>& tokens)
{
	if (tokens.empty()) return;

	for (const auto& token : tokens)
	{
		std::cout << "token (type: " << getTokenTypeStr(token.type) << ", value: ";

		if (tokenValueIsNotEmpty(token))
		{
			std::cout << token.text;
		}
		else
		{
			std::cout << "nulloptr";
		}
	}
}

std::vector<Token> amanises::Lexer::tokenize(std::string_view content)
{
	std::vector<Token> tokList;


	return tokList;
}

std::string amanises::Lexer::trimWhite(std::string& content)
{
	content.reserve(content.size());

	content.erase(std::remove_if(content.begin(), content.end(), [](char c) {
		return std::isspace(c);
		}), content.end());
	return content;
}

std::vector<std::string> amanises::Lexer::splitIntoChunks(const std::string& content, size_t maxChunkSize)
{
	// TODO: edgecases to solve:
	// tracking syntax boundaries. as in open and closed delimiters.
	// tokenization marks is a kinda todo

	std::vector<std::string> chunks;
	size_t contentSize = content.size();
	size_t start = 0;

	while (start < contentSize) {

		size_t end = std::min(start + maxChunkSize, contentSize);

		// ensure we do not split within a string literal or comment
		while (end < contentSize && !isBoundaryCharacter(content[end])) {
			++end;
		}

		chunks.push_back(content.substr(start, end - start));
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

bool amanises::Lexer::tokenValueIsNotEmpty(const Token& token)
{
	return token.text != nullptr && token.text[0] != '\0';
}

bool amanises::Lexer::isBoundaryCharacter(char c)
{
	// TODO: more boundary characters to be added
	return c == '\n' || c == '}' || c == '{' || c == ';';
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