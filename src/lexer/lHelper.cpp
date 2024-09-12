#include "lHelper.hpp"

std::string amanises::LexerHelper::tok_kind_to_str(const token_kind kind)
{
	switch (kind)
	{
		// reserved keywords

	case token_kind::TOK_IF:               return "IF";
	case token_kind::TOK_ELSE:             return "ELSE";
	case token_kind::TOK_FOR:              return "FOR";
	case token_kind::TOK_WHILE:            return "WHILE";
	case token_kind::TOK_DO:               return "DO";
	case token_kind::TOK_RETURN:           return "RETURN";
	case token_kind::TOK_BREAK:            return "BREAK";
	case token_kind::TOK_CONTINUE:         return "CONTINUE";
	case token_kind::TOK_SWITCH:           return "SWITCH";
	case token_kind::TOK_CASE:             return "CASE";
	case token_kind::TOK_DEFAULT:          return "DEFAULT";

	case token_kind::TOK_CLASS:            return "CLASS";
	case token_kind::TOK_PRIVATE:          return "PRIVATE";
	case token_kind::TOK_PROTECTED:        return "PROTECTED";
	case token_kind::TOK_PUBLIC:           return "PUBLIC";
	case token_kind::TOK_STATIC:           return "STATIC";

	case token_kind::TOK_CONST:            return "CONST";

	case token_kind::TOK_NEW:              return "NEW";
	case token_kind::TOK_DELETE:           return "DELETE";

		// data types
	case token_kind::TOK_I8:               return "I8";
	case token_kind::TOK_I16:              return "I16";
	case token_kind::TOK_I32:              return "I32";
	case token_kind::TOK_I64:              return "I64";
	case token_kind::TOK_I128:             return "I128";

	case token_kind::TOK_U8:               return "U8";
	case token_kind::TOK_U16:              return "U16";
	case token_kind::TOK_U32:              return "U32";
	case token_kind::TOK_U64:              return "U64";
	case token_kind::TOK_U128:             return "U128";

	case token_kind::TOK_F32:              return "F32";
	case token_kind::TOK_F64:              return "F64";

	case token_kind::TOK_CHAR:             return "CHAR";
	case token_kind::TOK_STRING:           return "STRING";
	case token_kind::TOK_BOOL:             return "BOOL";
	case token_kind::TOK_VOID:             return "VOID";

		// operators
	case token_kind::TOK_PLUS:             return "PLUS";
	case token_kind::TOK_MINUS:            return "MINUS";
	case token_kind::TOK_MULTIPLY:         return "MULTIPLY";
	case token_kind::TOK_DIVIDE:           return "DIVIDE";
	case token_kind::TOK_MODULO:           return "MODULO";

	case token_kind::TOK_INCREMENT:        return "INCREMENT";
	case token_kind::TOK_DECREMENT:        return "DECREMENT";

	case token_kind::TOK_EQUAL:            return "EQUAL";
	case token_kind::TOK_NOT_EQUAL:        return "NOT_EQUAL";
	case token_kind::TOK_LESS_THAN:        return "LESS_THAN";
	case token_kind::TOK_GREATER_THAN:     return "GREATER_THAN";
	case token_kind::TOK_LESS_EQUAL:       return "LESS_EQUAL";
	case token_kind::TOK_GREATER_EQUAL:    return "GREATER_EQUAL";

	case token_kind::TOK_AND:              return "AND";
	case token_kind::TOK_OR:               return "OR";
	case token_kind::TOK_NOT:              return "NOT";

	case token_kind::TOK_ASSIGN:           return "ASSIGN";
	case token_kind::TOK_PLUS_ASSIGN:      return "PLUS_ASSIGN";
	case token_kind::TOK_MINUS_ASSIGN:     return "MINUS_ASSIGN";
	case token_kind::TOK_MULTIPLY_ASSIGN:  return "MULTIPLY_ASSIGN";

	case token_kind::TOK_MEMBER_ACCESS:    return "MEMBER_ACCESS";
	case token_kind::TOK_POINTER_ACCESS:   return "POINTER_ACCESS";
	case token_kind::TOK_SCOPE:            return "SCOPE";

		// punctuation
	case token_kind::TOK_SEMICOLON:		   return "SEMICOLON";
	case token_kind::TOK_COLON:            return "COLON";
	case token_kind::TOK_DOT:              return "DOT";
	case token_kind::TOK_COMMA:            return "COMMA";
	case token_kind::TOK_OPEN_PAR:         return "OPEN_PAR";
	case token_kind::TOK_CLOSE_PAR:        return "CLOSE_PAR";
	case token_kind::TOK_OPEN_BRACE:       return "OPEN_BRACE";
	case token_kind::TOK_CLOSE_BRACE:      return "CLOSE_BRACE";
	case token_kind::TOK_OPEN_BRACKET:     return "OPEN_BRACKET";
	case token_kind::TOK_CLOSE_BRACKET:    return "CLOSE_BRACKET";

		// identifier
	case token_kind::TOK_IDENTIFIER:       return "IDENTIFIER";

		// literals 
	case token_kind::TOK_INTEGER_LIT:      return "INTEGER_LIT";
	case token_kind::TOK_FLOAT_LIT:        return "FLOAT_LIT";
	case token_kind::TOK_CHAR_LIT:         return "CHAR_LIT";
	case token_kind::TOK_STRING_LIT:       return "STRING_LIT";
	case token_kind::TOK_BOOLEAN_LIT:      return "BOOLEAN_LIT";
	case token_kind::TOK_NULL_LIT:         return "NULL_LIT";

		// comments
	case token_kind::TOK_LINE_COMMENT:     return "LINE_COMMENT";
	case token_kind::TOK_BLOCK_COMMENT:    return "BLOCK_COMMENT";

		// function related
	case token_kind::TOK_FUNCTION:         return "FUNCTION";
	case token_kind::TOK_METHOD:           return "METHOD";
	case token_kind::TOK_RETURN_TYPE:      return "RETURN_TYPE";

		// start and end of file
	case token_kind::TOK_EOF:              return "_EOF";
	case token_kind::TOK_SOF:              return "_SOF";

		// preprocessors
	case token_kind::TOK_PRAGMA:           return "PRAGMA";
	case token_kind::TOK_INCLUDE:          return "INCLUDE";

		// error handling
	case token_kind::TOK_ERROR:            return "ERROR";
	default:                               return "UNDEFINED";
	}
}

void amanises::LexerHelper::print_tokens_verbose(std::vector<Token>& tok_list)
{
	if (tok_list.empty()) return;

	for (Token& token : tok_list)
	{
		std::cout << token_to_str_verbose(&token) << std::endl;
	}
}

void amanises::LexerHelper::print_tokens_non_verbose(std::vector<Token>& tok_list)
{
	if (tok_list.empty()) return;

	for (Token& token : tok_list)
	{
		std::cout << token_to_str_non_verbose(&token) << std::endl;
	}
}

std::string amanises::LexerHelper::token_to_str_verbose(Token* token)
{
	const std::string tok_kind = tok_kind_to_str(token->kind);
	std::string tok_val = token->val.has_value() ? token->val.value() : "null";

	const std::string tok_template =
		"Tok <k=`" + tok_kind + "`" +
		", v=`" + tok_val + "`" +
		", l=`" + std::to_string(token->line) + "`" +
		", c=`" + std::to_string(token->col) + "`>";

	return tok_template;
}

std::string amanises::LexerHelper::token_to_str_non_verbose(Token* token)
{
	const std::string tok_kind = tok_kind_to_str(token->kind);
	std::string tok_val = token->val.has_value() ? token->val.value() : "null";

	const std::string tok_template =
		"Tok <k=`" + tok_kind + "`" +
		", v=`" + tok_val + "`>";

	return tok_template;
}

bool amanises::LexerHelper::peek_ahead(const std::string_view& content, size_t& idx, char to_check)
{
	if (idx + 1 < content.size() && content[idx + 1] == to_check)
	{
		return true;
	}
	return false;
}

bool amanises::LexerHelper::is_chunk_buf_boundary_char(char c)
{
	switch (c)
	{
	case '\n':
	case '}':
	case '{':
	case ';':
		return true;
	default:
		return false;
	}
}

bool amanises::LexerHelper::is_space(char c)
{
	switch (c)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		return true;
	default:
		return false;
	}
}

bool amanises::LexerHelper::is_preproc_start(char c)
{
	return c == '#';
}

bool amanises::LexerHelper::is_alpha(char c)
{
	return std::isalpha(static_cast<unsigned char>(c));
}

bool amanises::LexerHelper::is_alpha_num(char c)
{
	return std::isalnum(static_cast<unsigned char>(c));
}

bool amanises::LexerHelper::is_digit(char c)
{
	return std::isdigit(static_cast<unsigned char>(c));
}

bool amanises::LexerHelper::is_operator(const std::string_view& content, size_t& idx)
{
	char c = content[idx];

	switch (c)
	{
	case '+':
		if (peek_ahead(content, idx, '+'))
		{
			return true;
		}
		if (peek_ahead(content, idx, '='))
		{
			return true;
		}
		return true;
	case '-':
		if (peek_ahead(content, idx, '-'))
		{
			return true;
		}
		if (peek_ahead(content, idx, '='))
		{
			return true;
		}
		if (peek_ahead(content, idx, '>'))
		{
			return true;
		}
		return true;
	case '*':
		if (peek_ahead(content, idx, '='))
		{
			return true;
		}
		return true;
	case '/':
	case '%':
		return true;
	case '=':
		if (peek_ahead(content, idx, '='))
		{
			return true;
		}
		return true;
	case '!':
		if (peek_ahead(content, idx, '='))
		{
			return true;
		}
		return true;
	case '<':
		if (peek_ahead(content, idx, '='))
		{
			return true;
		}

		return true;
	case '>':
		if (peek_ahead(content, idx, '='))
		{
			return true;
		}
		return true;
	case '&':
		if (peek_ahead(content, idx, '&'))
		{
			return true;
		}
		return true;
	case '|':
		if (peek_ahead(content, idx, '|'))
		{
			return true;
		}
		return true;
	default:
		return false;
	}
}

bool amanises::LexerHelper::is_punctuator(char c)
{
	switch (c)
	{
	case ';':
	case ':':
	case '.':
	case ',':
	case '(':
	case ')':
	case '{':
	case '}':
	case '[':
	case ']':
		return true;
	default:
		return false;
	}
}

bool amanises::LexerHelper::is_identifier_start(char c)
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_'
		|| (c >= 128);
}

bool amanises::LexerHelper::is_identifier(char c)
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '_'
		|| (c >= 128);
}

bool amanises::LexerHelper::is_literal_start(char c)
{
	return (is_digit(c) || c == '"');
}

bool amanises::LexerHelper::is_literal(char c)
{
	// identifier chars can be in stuff like strings etc.
	return (is_digit(c) || c == '.' || is_identifier(c));
}

std::string amanises::LexerHelper::trim_str(const std::string& str)
{
	auto start = str.find_first_not_of(" \t\n\r\f\v");
	auto end = str.find_last_not_of(" \t\n\r\f\v");
	return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::string amanises::LexerHelper::trim_white_space_str(std::string& str)
{
	return std::string();
}
