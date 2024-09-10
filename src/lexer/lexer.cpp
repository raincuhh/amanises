#include "lexer.hpp"

// #include "testLib.h"
//
//int main() {
//	print("Hello World!");
//	return 0;
//}

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
	init_token_map();
}

bool amanises::Lexer::tokenize_content()
{
	// chunking
	const size_t buf_size = 8192; // TODO: might eventually make the BUFFER_SIZE be dynamically set between 8kb and 16kb
	std::vector<std::string> chnk_bufs = split_to_chunk_buffers(m_content, buf_size);

	for (const std::string& buf : chnk_bufs)
	{
		// tokenize to buf token list
		std::vector<Token> buf_tok_list;
		tokenize(buf, buf_tok_list);

		// move buf token list to full token list
		full_tok_list.reserve(full_tok_list.size() + buf_tok_list.size());
		std::move(buf_tok_list.begin(), buf_tok_list.end(), std::back_inserter(full_tok_list));
	}

	// check if the full token list is still empty after tokenization
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

	for (Token& token : tokens)
	{
		std::cout << token_to_str(&token) << std::endl;
	}
}

void amanises::Lexer::tokenize(std::string_view content, std::vector<Token>& tok_list)
{
	lex_states lex_state = lex_states::LEX_INITIAL;
	std::string tok_buf;

	for (size_t idx = 0; idx < content.length();)
	{
		Token tok = get_next_token(content, idx, lex_state, tok_buf, tok_list);

		std::cout << tok_buf << std::endl;
		tok_list.push_back(tok);
		clear_token_buffer(tok_buf);

		idx++;
	}
	tok_list.push_back(Token{ .kind = token_kind::TOK_EOF });
}

Token amanises::Lexer::get_next_token(std::string_view content, size_t& idx, lex_states& lex_state, std::string& tok_buf, std::vector<Token>& tok_list)
{
	while (idx < content.length())
	{
		char c = content[idx];

		switch (lex_state)
		{
		case lex_states::LEX_INITIAL:
			if (is_space(c))
			{
				lex_state = lex_states::LEX_WHITE_SPACE;
				break;
			}
			else if (is_alpha(c) || c == '_')
			{
				lex_state = lex_states::LEX_IDENTIFIER;
				continue;
			}
			else if (is_digit(c) || c == '"')
			{
				lex_state = lex_states::LEX_LITERAL;
				continue;
			}
			else if (is_operator(content, idx))
			{
				lex_state = lex_states::LEX_OPERATOR;
				continue;
			}
			else if (is_punctuator(c))
			{
				lex_state = lex_states::LEX_PUNCTUATION;
				continue;
			}
			else if (is_preproc(c))
			{
				lex_state = lex_states::LEX_PREPROC;
				continue;
			}
			else
			{
				lex_state = lex_states::LEX_ERROR;
				continue;
			}
		case lex_states::LEX_OPERATOR:
			accumulate_operator_token(content, idx, c, tok_buf, lex_state, tok_list);
			return Token{ .kind = determine_tok_kind(tok_buf), .val = tok_buf };
		case lex_states::LEX_PUNCTUATION:
			accumulate_punctuation_token(content, idx, c, tok_buf, lex_state, tok_list);
			return Token{ .kind = determine_tok_kind(tok_buf), .val = tok_buf };
		case lex_states::LEX_IDENTIFIER:
			accumulate_identifier_token(content, idx, c, tok_buf, lex_state, tok_list);
			return Token{ .kind = determine_tok_kind(tok_buf), .val = tok_buf };
		case lex_states::LEX_LITERAL:
			accumulate_literal_token(content, idx, c, tok_buf, lex_state, tok_list);
			return Token{ .kind = determine_literal_tok_kind(tok_buf), .val = tok_buf};
		case lex_states::LEX_PREPROC:
			accumulate_preproc_token(content, idx, c, tok_buf, lex_state, tok_list);
			return Token{ .kind = determine_tok_kind(tok_buf), .val = tok_buf };
		case lex_states::LEX_COMMENTS:
			handle_comments(content, idx, c, tok_buf, lex_state, tok_list);
			break;
		case lex_states::LEX_WHITE_SPACE:
			handle_white_space(content, idx, c, tok_buf, lex_state, tok_list);
			break;
		case lex_states::LEX_ERROR:
			std::cout << "error" << std::endl;
			break;
		}
	}
}

token_kind amanises::Lexer::determine_tok_kind(std::string& tok_buf)
{
	// make sure the word doesnt contain any hidden chars
	tok_buf = trim_word(tok_buf);

	auto it = tokMap.find(tok_buf);

	if (it != tokMap.end()) 
	{
		return it->second; 
	}
	if (it == tokMap.end())
	{
		return token_kind::TOK_IDENTIFIER;
	}
	else {
		return token_kind::TOK_ERROR;
	}
}

token_kind amanises::Lexer::determine_literal_tok_kind(std::string& tok_buf)
{
	// null literal
	if (tok_buf == "null") {
		return token_kind::TOK_NULL_LIT;
	}

	// string literal
	if (!tok_buf.empty() && tok_buf.front() == '"' && tok_buf.back() == '"') {
		return token_kind::TOK_STRING_LIT;
	}

	// float literal
	bool has_dot = tok_buf.find('.') != std::string::npos;
	bool is_float = has_dot && tok_buf.find_first_not_of("0123456789.") == std::string::npos;
	if (is_float) {
		return token_kind::TOK_FLOAT_LIT;
	}

	// integer literal
	bool is_integer = tok_buf.find_first_not_of("0123456789") == std::string::npos;
	if (is_integer) {
		return token_kind::TOK_INTEGER_LIT;
	}

	return token_kind::TOK_ERROR;
}

std::vector<std::string> amanises::Lexer::split_to_chunk_buffers(const std::string& content, size_t max_chunk_size)
{
	// TODO: edgecases to solve:
	// tracking syntax boundaries. as in open and closed delimiters.
	// tokenization marks kinda
	// dont know if it is actually something i need to solve, just test this later

	std::vector<std::string> chunks;
	size_t content_size = content.size();
	size_t start = 0;

	while (start < content_size) {
		size_t end = std::min(start + max_chunk_size, content_size);

		// ensure we only split on a boundary character
		while (end < content_size && !is_chunk_buf_boundary_char(content[end])) {
			++end;
		}

		// get the chunk
		std::string chunk = content.substr(start, end - start);
		chunks.push_back(chunk);
		std::cout << "chunkSize: " << content_size << std::endl;

		start = end;
	}
	return chunks;
}

std::string amanises::Lexer::trim_white_space(std::string& content)
{
	content.reserve(content.size());

	content.erase(std::remove_if(content.begin(), content.end(), [](char c) {
		return std::isspace(c);
		}), content.end());
	return content;
}

std::string amanises::Lexer::trim_word(const std::string& str)
{
	auto start = str.find_first_not_of(" \t\n\r\f\v");
	auto end = str.find_last_not_of(" \t\n\r\f\v");
	return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::string amanises::Lexer::token_kind_to_str(const token_kind type)
{
	switch (type)
	{
	// reserved keywords
	case token_kind::TOK_IF:               return "IF";
	case token_kind::TOK_ELSE:             return "ELSE";
	case token_kind::TOK_FOR:              return "FOR";
	case token_kind::TOK_WHILE:            return "WHILE";
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

	case token_kind::TOK_NEW:              return "NEW";
	case token_kind::TOK_DELETE:           return "DELETE";

	// data types
	case token_kind::TOK_INT:              return "INT";
	case token_kind::TOK_FLOAT:            return "FLOAT";
	case token_kind::TOK_DOUBLE:           return "DOUBLE";
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

std::string amanises::Lexer::token_to_str(Token* token)
{
	const std::string tok_kind = token_kind_to_str(token->kind);

	std::string tok_val = token->val.has_value() ? token->val.value() : "null";

	const std::string tok_template =
		"TOK <kind=" + tok_kind +
		", val=" + tok_val +
		", line=" + std::to_string(token->line) +
		", col=" + std::to_string(token->col) + ">";

	return tok_template;
}

void amanises::Lexer::init_token_map()
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
		{ "%", token_kind::TOK_MODULO },

		{ "++", token_kind::TOK_INCREMENT },
		{ "--", token_kind::TOK_DECREMENT },

		{ "==", token_kind::TOK_EQUAL },
		{ "!=", token_kind::TOK_NOT_EQUAL },
		{ "<", token_kind::TOK_LESS_THAN },
		{ ">", token_kind::TOK_GREATER_THAN },
		{ "<=", token_kind::TOK_LESS_EQUAL },
		{ ">=", token_kind::TOK_GREATER_EQUAL },

		{ "&&", token_kind::TOK_AND },
		{ "||", token_kind::TOK_OR },
		{ "!", token_kind::TOK_NOT },

		{ "=", token_kind::TOK_ASSIGN },
		{ "+=", token_kind::TOK_PLUS_ASSIGN },
		{ "-=", token_kind::TOK_MINUS_ASSIGN },
		{ "*=", token_kind::TOK_MULTIPLY_ASSIGN },

		{ ".", token_kind::TOK_MEMBER_ACCESS },
		{ "->", token_kind::TOK_POINTER_ACCESS },
		{ "::", token_kind::TOK_SCOPE },

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
		//{ "double", token_kind::TOK_DOUBLE },
		{ "char", token_kind::TOK_CHAR },
		{ "string", token_kind::TOK_STRING },
		{ "bool", token_kind::TOK_BOOL },
		{ "void", token_kind::TOK_VOID },

		// preprocessor
		{ "#pragma", token_kind::TOK_PRAGMA },
		{ "#include", token_kind::TOK_INCLUDE },
	};
}

void amanises::Lexer::update_line_col()
{
}

inline bool amanises::Lexer::peek_ahead(const std::string_view& content, size_t& idx, char to_check)
{
	if (idx + 1 < content.size() && content[idx + 1] == to_check)
	{
		return true;
	}
	return false;
}

inline bool amanises::Lexer::is_chunk_buf_boundary_char(char c)
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

inline bool amanises::Lexer::is_space(char c)
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

inline bool amanises::Lexer::is_preproc(char c)
{
	return c == '#';
}

inline bool amanises::Lexer::is_alpha(char c)
{
	return std::isalpha(static_cast<unsigned char>(c));
}

inline bool amanises::Lexer::is_alpha_num(char c)
{
	return std::isalnum(static_cast<unsigned char>(c));
}

inline bool amanises::Lexer::is_operator(const std::string_view& content, size_t& idx)
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

inline bool amanises::Lexer::is_punctuator(char c)
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


bool amanises::Lexer::is_identifier_start(char c)
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_'
		|| (c >= 128);
}

bool amanises::Lexer::is_identifier_char(char c)
{
	return (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9')
		|| c == '_'
		|| (c >= 128);
}

inline bool amanises::Lexer::is_digit(char c)
{
	return std::isdigit(static_cast<unsigned char>(c));
}

bool amanises::Lexer::is_literal_start(char c)
{
	return (is_digit(c) || c == '"');
}

bool amanises::Lexer::is_literal_char(char c)
{
	return (is_digit(c) || c == '.' || is_identifier_char(c)); // identifier chars can be in stuff like strings etc.
}

void amanises::Lexer::accumulate_preproc_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list)
{
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	idx++;

	while (idx < content.length() && is_alpha_num(content[idx]))
	{
		tok_buf.push_back(content[idx]);
		idx++;
	}

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_operator_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list)
{
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	idx++;

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_punctuation_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list)
{
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	idx++;

	while (idx < content.length() && is_punctuator(c))
	{
		tok_buf.push_back(c);
		idx++;

	}
	std::cout << tok_buf << std::endl;

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_identifier_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list)
{
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	idx++;

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_literal_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list)
{
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);

	bool is_string_literal = (c == '"');
	idx++;

	while (idx < content.length())
	{
		c = content[idx];

		if (is_string_literal)
		{
			tok_buf.push_back(c);
			idx++;

			if (c == '"')
			{
				break;
			}
		}
		else if (is_literal_char(c))
		{
			tok_buf.push_back(c);
			idx++;
		}
		else
		{
			break;
		}
	}
	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::handle_comments(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list)
{
}

void amanises::Lexer::handle_white_space(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list)
{
	while (idx < content.length() && is_space(content[idx]))
	{
		idx++;
	}
	lex_state = lex_states::LEX_INITIAL;
}