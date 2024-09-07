#include "lexer.hpp"

amanises::Lexer::Lexer(std::string content, size_t _contentLen, Logger* logger) :
	m_logger(logger),
	m_content(std::move(content)),
	m_content_len(),
	m_cursor(),
	m_line(),
	m_col(),

	full_tok_list(),
	tokMap()
{
	init_u_tok_map();
}

bool amanises::Lexer::lex_content()
{
	// splits up source into chunks for tokenization
	const size_t buf_size = 8192; // TODO: might eventually make the BUFFER_SIZE be dynamically set between 8kb and 16kb
	std::vector<std::string> c_buffs = split_to_buffers(m_content, buf_size);

	for (const std::string& buf : c_buffs)
	{
		// tokenize to a buffer token list
		std::vector<Token> buf_tok_list;
		tokenize(buf, buf_tok_list);

		// reserve and move the buffer token list to the back of full token list 
		full_tok_list.reserve(full_tok_list.size() + buf_tok_list.size());
		std::move(buf_tok_list.begin(), buf_tok_list.end(), std::back_inserter(full_tok_list));

	}

	// check if the full token list is still empty after tokenization, (should atleast have a EOF token is why)
	if (full_tok_list.empty())
	{
		m_logger->log(log_type::ERROR, "Full token list empty after tokenization.");
		return false; 
	}

	return true;
}

void amanises::Lexer::debug_print_tokens(std::vector<Token>& tokens)
{
	if (tokens.empty()) return;

	for (const auto& token : tokens)
	{
		std::cout << "token (type: " << get_token_kind_str(token.kind) << ", value: ";

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

void amanises::Lexer::tokenize(std::string_view content, std::vector<Token>& token_list)
{
	using state = amanises::Lexer::lex_states;

	lex_states lex_state = state::LEX_INITIAL;
	std::string tok_buf;

	std::cout << content << std::endl;

	for (size_t i = 0; i < content.length(); i++)
	{
		char c = content[i];
		//std::cout << c << std::endl;

		switch (lex_state)
		{
		case state::LEX_INITIAL:
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
		case state::LEX_PREPROC:

			
			break;
		case state::LEX_COMMENTS:

			
			break;
		case state::LEX_KEYWORD:


			break;
		case state::LEX_DATA_TYPE:


			break;
		case state::LEX_OPERATOR:


			break;
		case state::LEX_PUNCTUATION:


			break;

		case state::LEX_IDENTIFIER:


			break;
		case state::LEX_LITERALS:


			break;
		default:

			break;
		}
	}

	token_list.push_back(Token{ .kind = token_kind::TOK_EOF });
}

std::string amanises::Lexer::trim_white_space(std::string& content)
{
	content.reserve(content.size());

	content.erase(std::remove_if(content.begin(), content.end(), [](char c) {
		return std::isspace(c);
		}), content.end());
	return content;
}

std::vector<std::string> amanises::Lexer::split_to_buffers(const std::string& content, size_t max_chunk_size)
{
	// TODO: edgecases to solve:
	// tracking syntax boundaries. as in open and closed delimiters.
	// tokenization marks kinda

	std::vector<std::string> chunks;
	size_t content_size = content.size();
	size_t start = 0;

	while (start < content_size) {

		size_t end = std::min(start + max_chunk_size, content_size);

		// ensure we only split on a boundary character
		while (end < content_size && !Utils::isBoundaryCharacter(content[end])) {
			++end;
		}

		// get the chunk
		std::string chunk = content.substr(start, end - start);
		chunks.push_back(chunk);

		// debug, printing out chunk size
		std::cout << "chunkSize: " << content_size << std::endl;

		start = end;
	}
	return chunks;
}

std::string amanises::Lexer::get_token_kind_str(const token_kind type)
{
	switch (type)
	{
	// reserved keywords
	case token_kind::TOK_IF:            return "IF";
	case token_kind::TOK_ELSE:          return "ELSE";
	case token_kind::TOK_FOR:           return "FOR";
	case token_kind::TOK_WHILE:         return "WHILE";
	case token_kind::TOK_RETURN:        return "RETURN";
	case token_kind::TOK_BREAK:         return "BREAK";
	case token_kind::TOK_CONTINUE:      return "CONTINUE";
	case token_kind::TOK_SWITCH:        return "SWITCH";
	case token_kind::TOK_CASE:          return "CASE";
	case token_kind::TOK_DEFAULT:       return "DEFAULT";

	case token_kind::TOK_CLASS:         return "CLASS";
	case token_kind::TOK_PRIVATE:       return "PRIVATE";
	case token_kind::TOK_PROTECTED:     return "PROTECTED";
	case token_kind::TOK_PUBLIC:        return "PUBLIC";
	case token_kind::TOK_STATIC:        return "STATIC";

	case token_kind::TOK_NEW:           return "NEW";
	case token_kind::TOK_DELETE:        return "DELETE";

	// data types
	case token_kind::TOK_INT:           return "INT";
	case token_kind::TOK_FLOAT:         return "FLOAT";
	case token_kind::TOK_DOUBLE:        return "DOUBLE";
	case token_kind::TOK_CHAR:          return "CHAR";
	case token_kind::TOK_STRING:        return "STRING";
	case token_kind::TOK_BOOL:          return "BOOL";
	case token_kind::TOK_VOID:          return "VOID";

	// operators
	case token_kind::TOK_PLUS:          return "PLUS";
	case token_kind::TOK_MINUS:         return "MINUS";
	case token_kind::TOK_MULTIPLY:      return "MULTIPLY";
	case token_kind::TOK_DIVIDE:        return "DIVIDE";
	case token_kind::TOK_ASSIGN:        return "ASSIGN";
	case token_kind::TOK_EQUAL:         return "EQUAL";
	case token_kind::TOK_NOT_EQUAL:     return "NOT_EQUAL";
	case token_kind::TOK_LESS_THAN:     return "LESS_THAN";
	case token_kind::TOK_GREATER_THAN:  return "GREATER_THAN";
	case token_kind::TOK_LESS_EQUAL:    return "LESS_EQUAL";
	case token_kind::TOK_GREATER_EQUAL: return "GREATER_EQUAL";
	case token_kind::TOK_AND:           return "AND";
	case token_kind::TOK_OR:            return "OR";
	case token_kind::TOK_NOT:           return "NOT";
	case token_kind::TOK_INCREMENT:     return "INCREMENT";
	case token_kind::TOK_DECREMENT:     return "DECREMENT";

	// punctuation
	case token_kind::TOK_SEMICOLON:     return "SEMICOLON";
	case token_kind::TOK_COLON:         return "COLON";
	case token_kind::TOK_DOT:           return "DOT";
	case token_kind::TOK_COMMA:         return "COMMA";
	case token_kind::TOK_OPEN_PAR:      return "OPEN_PAR";
	case token_kind::TOK_CLOSE_PAR:     return "CLOSE_PAR";
	case token_kind::TOK_OPEN_BRACE:    return "OPEN_BRACE";
	case token_kind::TOK_CLOSE_BRACE:   return "CLOSE_BRACE";
	case token_kind::TOK_OPEN_BRACKET:  return "OPEN_BRACKET";
	case token_kind::TOK_CLOSE_BRACKET: return "CLOSE_BRACKET";

	// identifier
	case token_kind::TOK_IDENTIFIER:    return "IDENTIFIER";

	// literals
	case token_kind::TOK_INTEGER_LIT:   return "INTEGER_LIT";
	case token_kind::TOK_FLOAT_LIT:     return "FLOAT_LIT";
	case token_kind::TOK_CHAR_LIT:      return "CHAR_LIT";
	case token_kind::TOK_STRING_LIT:    return "STRING_LIT";
	case token_kind::TOK_BOOLEAN_LIT:   return "BOOLEAN_LIT";
	case token_kind::TOK_NULL_LIT:      return "NULL_LIT";

	// comments
	case token_kind::TOK_LINE_COMMENT:  return "LINE_COMMENT";
	case token_kind::TOK_BLOCK_COMMENT: return "BLOCK_COMMENT";

	// function related
	case token_kind::TOK_FUNCTION:      return "FUNCTION";
	case token_kind::TOK_METHOD:        return "METHOD";
	case token_kind::TOK_RETURN_TYPE:   return "RETURN_TYPE";

	// start and end of file
	case token_kind::TOK_EOF:           return "_EOF";
	case token_kind::TOK_SOF:           return "_SOF";

	// preprocessors
	case token_kind::TOK_PRAGMA:        return "PRAGMA";
	case token_kind::TOK_INCLUDE:       return "INCLUDE";

	// error handling
	case token_kind::TOK_ERROR:         return "ERROR";
	default:                           return "UNDEFINED";
	}
}

void amanises::Lexer::init_u_tok_map()
{
	tokMap = {
		// punctuation
		{ ";", token_kind::TOK_SEMICOLON },
		{ ":", token_kind::TOK_COLON },
		{ ".", token_kind::TOK_DOT },
		{ ",", token_kind::TOK_COMMA },
		{ "(", token_kind::TOK_OPEN_PAR },
		{ ")", token_kind::TOK_CLOSE_PAR },
		{ "{", token_kind::TOK_OPEN_BRACE },
		{ "}", token_kind::TOK_CLOSE_BRACE },
		{ "[", token_kind::TOK_OPEN_BRACKET },
		{ "]", token_kind::TOK_CLOSE_BRACKET },

		// operators
		{ "+", token_kind::TOK_PLUS },
		{ "-", token_kind::TOK_MINUS },
		{ "*", token_kind::TOK_MULTIPLY },
		{ "/", token_kind::TOK_DIVIDE },
		{ "=", token_kind::TOK_ASSIGN },
		{ "==", token_kind::TOK_EQUAL },
		{ "!=", token_kind::TOK_NOT_EQUAL },
		{ "<", token_kind::TOK_LESS_THAN },
		{ ">", token_kind::TOK_GREATER_THAN },
		{ "<=", token_kind::TOK_LESS_EQUAL },
		{ ">=", token_kind::TOK_GREATER_EQUAL },
		{ "&&", token_kind::TOK_AND },
		{ "||", token_kind::TOK_OR },
		{ "!", token_kind::TOK_NOT },
		{ "++", token_kind::TOK_INCREMENT },
		{ "--", token_kind::TOK_DECREMENT },

		// keywords
		{ "if", token_kind::TOK_IF },
		{ "else", token_kind::TOK_ELSE },
		{ "for", token_kind::TOK_FOR },
		{ "while", token_kind::TOK_WHILE },
		{ "return", token_kind::TOK_RETURN },
		{ "break", token_kind::TOK_BREAK },
		{ "continue", token_kind::TOK_CONTINUE },
		{ "switch", token_kind::TOK_SWITCH },
		{ "case", token_kind::TOK_CASE },
		{ "default", token_kind::TOK_DEFAULT },

		{ "class", token_kind::TOK_CLASS },
		{ "private", token_kind::TOK_PRIVATE },
		{ "protected", token_kind::TOK_PROTECTED },
		{ "public", token_kind::TOK_PUBLIC },
		{ "static", token_kind::TOK_STATIC },

		{ "new", token_kind::TOK_NEW },
		{ "delete", token_kind::TOK_DELETE },

		// data Types
		{ "int", token_kind::TOK_INT },
		{ "float", token_kind::TOK_FLOAT },
		{ "double", token_kind::TOK_DOUBLE },
		{ "char", token_kind::TOK_CHAR },
		{ "string", token_kind::TOK_STRING },
		{ "bool", token_kind::TOK_BOOL },
		{ "void", token_kind::TOK_VOID },

		// preprocessors
		{ "#pragma", token_kind::TOK_PRAGMA },
		{ "#include", token_kind::TOK_INCLUDE },

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