#include "lexer.hpp"

amanises::Lexer::Lexer(Logger* logger) :
	m_logger(logger),
	m_cur_line(),
	m_cur_col(),
	m_tok_map() {
	init_token_map();
}

// responsible for tokenizing a specific source file.
std::vector<Token> amanises::Lexer::tokenize_source_file(std::string src) {
	std::vector<Token> file_tok_list;
	const size_t buf_size = 8192; // TODO: might eventually make the BUFFER_SIZE be dynamically set between 8kb and 16kb
	size_t line = 1;
	size_t col = 0;

	m_cur_line = line;
	m_cur_col = col;

	//file_tok_list.push_back(Token{ .kind = token_kind::TOK_SOF });

	// splitting the source into chunks of 8192
	std::vector<std::string> chunk_bufs = split_to_chunk_buffers(std::move(src), buf_size);
	for (const std::string& buf : chunk_bufs) {
		// tokenize to buf token list, and move it to the full token list
		std::vector<Token> buf_tok_list;
		tokenize(buf, buf_tok_list);

		file_tok_list.reserve(file_tok_list.size() + buf_tok_list.size());
		std::move(buf_tok_list.begin(), buf_tok_list.end(), std::back_inserter(file_tok_list));
	}

	file_tok_list.push_back(Token{ .kind = token_kind::TOK_EOF });

	return file_tok_list;
}

void amanises::Lexer::tokenize(std::string_view content, std::vector<Token>& tok_list) {
	lex_states lex_state = lex_states::LEX_INITIAL;
	std::string tok_buf;

	for (size_t idx = 0; idx < content.length();) {
		Token tok = get_next_token(content, idx, lex_state, tok_buf, tok_list);

		if (!tok_buf.empty()) {

			//std::cout << tok_buf << std::endl; //debug
			tok_list.push_back(tok);
			clear_token_buffer(tok_buf);
		}
	}
}

Token amanises::Lexer::get_next_token(std::string_view content, size_t& idx, lex_states& lex_state, std::string& tok_buf, std::vector<Token>& tok_list) {
	while (idx < content.length()) {
		char c = content[idx];

		switch (lex_state) {
		case lex_states::LEX_INITIAL:
			if (LexerHelper::is_space(c)) {
				lex_state = lex_states::LEX_WHITE_SPACE;
				break;
			}
			else if (LexerHelper::is_identifier_start(c)) {
				lex_state = lex_states::LEX_IDENTIFIER;
				continue;
			}
			else if (LexerHelper::is_literal_start(c)) {
				lex_state = lex_states::LEX_LITERAL;
				continue;
			}
			else if (LexerHelper::is_operator(content, idx)) {
				if (LexerHelper::peek_ahead(content, idx, '*') || LexerHelper::peek_ahead(content, idx, '/')) {
					lex_state = lex_states::LEX_COMMENTS;
					continue;
				}
				lex_state = lex_states::LEX_OPERATOR;
				continue;
			}
			else if (LexerHelper::is_punctuator(c)) {
				lex_state = lex_states::LEX_PUNCTUATION;
				continue;
			}
			else if (LexerHelper::is_preproc_start(c)) {
				lex_state = lex_states::LEX_PREPROC;
				continue;
			}
			else {
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

token_kind amanises::Lexer::determine_tok_kind(std::string& tok_buf) {
	tok_buf = LexerHelper::trim_str(tok_buf);

	auto it = m_tok_map.find(tok_buf);

	if (it != m_tok_map.end()) {
		return it->second; 
	}
	if (it == m_tok_map.end()) {
		return token_kind::TOK_IDENTIFIER;
	}
	else {
		return token_kind::TOK_ERROR;
	}
}

token_kind amanises::Lexer::determine_literal_tok_kind(std::string& tok_buf) {
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

std::vector<std::string> amanises::Lexer::split_to_chunk_buffers(const std::string& content, size_t max_chunk_size) {
	// TODO: tracking syntax boundaries. as in open and closed delimiters.
	// TODO: tokenization marks kinda
	// dont know if it is actually something i need to solve, just test this later

	std::vector<std::string> chunks;
	size_t content_size = content.size();
	size_t start = 0;

	while (start < content_size) {
		size_t end = std::min(start + max_chunk_size, content_size);

		// ensure we only split on a boundary character
		while (end < content_size && !LexerHelper::is_chunk_buf_boundary_char(content[end])) {
			++end;
		}

		// get the chunk
		std::string chunk = content.substr(start, end - start);
		chunks.push_back(chunk);
		//std::cout << "chunkSize: " << content_size << std::endl; // debug

		start = end;
	}
	return chunks;
}

void amanises::Lexer::init_token_map() {
	m_tok_map = {
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
		{ "->", token_kind::TOK_L_ARROW },
		{ "::", token_kind::TOK_SCOPE },

		// keywords
		{ "if", token_kind::TOK_IF },
		{ "else", token_kind::TOK_ELSE },
		{ "for", token_kind::TOK_FOR },
		{ "while", token_kind::TOK_WHILE },
		{ "do", token_kind::TOK_DO },
		{ "proc", token_kind::TOK_FUNCTION },
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

		{ "const", token_kind::TOK_CONST },

		{ "new", token_kind::TOK_NEW },
		{ "delete", token_kind::TOK_DELETE },

		// data Types
		{ "i8", token_kind::TOK_I8 },
		{ "i16", token_kind::TOK_I16 },
		{ "i32", token_kind::TOK_I32 },
		{ "i64", token_kind::TOK_I64 },
		{ "i128", token_kind::TOK_I128 },

		{ "u8", token_kind::TOK_U8 },
		{ "u16", token_kind::TOK_U16 },
		{ "u32", token_kind::TOK_U32 },
		{ "u64", token_kind::TOK_U64 },
		{ "u128", token_kind::TOK_U128 },

		{ "f32", token_kind::TOK_F32 },
		{ "f64", token_kind::TOK_F64 },

		{ "char", token_kind::TOK_CHAR },
		{ "string", token_kind::TOK_STRING },
		{ "bool", token_kind::TOK_BOOL },
		{ "void", token_kind::TOK_VOID },

		// preprocessor
		{ "#pragma", token_kind::TOK_PRAGMA },
		{ "#include", token_kind::TOK_INCLUDE },
	};
}

void amanises::Lexer::update_line_col(const std::string_view& content, size_t& idx) {
	if (content[idx] == '\n') {
		m_cur_line++;
		m_cur_col = 0;
	}
	else  {
		m_cur_col++;
	}
}

void amanises::Lexer::accumulate_preproc_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list) {
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	update_line_col(content, idx);
	idx++;

	while (idx < content.length() && LexerHelper::is_alpha_num(content[idx])) {
		tok_buf.push_back(content[idx]);
		update_line_col(content, idx);
		idx++;
	}

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_operator_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list) {
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	update_line_col(content, idx);
	idx++;

	while (idx < content.length() && LexerHelper::is_operator(content, idx)) {
		tok_buf.push_back(content[idx]);
		update_line_col(content, idx);
		idx++;
	}

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_punctuation_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list) {
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	update_line_col(content, idx);

	idx++;
	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_identifier_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list) {
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	update_line_col(content, idx);
	idx++;

	while (idx < content.length() && LexerHelper::is_identifier(content[idx])) {
		tok_buf.push_back(content[idx]);
		update_line_col(content, idx);
		idx++;
	}

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::accumulate_literal_token(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list) {
	clear_token_buffer(tok_buf);
	tok_buf.push_back(c);
	update_line_col(content, idx);
	idx++;

	bool is_string_literal = (c == '"');

	while (idx < content.length()) {
		c = content[idx];

		if (is_string_literal) {
			tok_buf.push_back(c);
			update_line_col(content, idx);
			idx++;

			if (c == '"') {
				break;
			}
		}
		else if (LexerHelper::is_literal(c)) {
			tok_buf.push_back(c);
			update_line_col(content, idx);
			idx++;
		}
		else {
			break;
		}
	}

	lex_state = lex_states::LEX_INITIAL;
}

void amanises::Lexer::handle_comments(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list) {
	clear_token_buffer(tok_buf);

	while (idx < content.length()) {
		if (content[idx] == '*' && LexerHelper::peek_ahead(content, idx, '/')) {
			idx += 2;
			break;
		}
		else if (content[idx] == '\n') {
			idx++;
			break;
		}
		idx++;
	}

	lex_state = lex_states::LEX_COMMENTS;
}

void amanises::Lexer::handle_white_space(const std::string_view& content, size_t& idx, char& c, std::string& tok_buf, lex_states& lex_state, std::vector<Token>& tok_list) {
	while (idx < content.length() && LexerHelper::is_space(content[idx])) {
		update_line_col(content, idx);
		idx++;
	}

	lex_state = lex_states::LEX_INITIAL;
}