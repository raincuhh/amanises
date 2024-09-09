#pragma once

#include <iostream>
#include <string>
#include <optional>

enum class token_kind {
	// start and end of file
	TOK_EOF = 0,
	TOK_SOF,

	// preprocessors
	TOK_PRAGMA,
	TOK_INCLUDE,

	// comments
	TOK_LINE_COMMENT,
	TOK_BLOCK_COMMENT,

	// function related
	TOK_FUNCTION,
	TOK_METHOD,
	TOK_RETURN_TYPE,

	// reserved keywords
	TOK_IF,
	TOK_ELSE,
	TOK_FOR,
	TOK_WHILE,
	TOK_RETURN,
	TOK_BREAK,
	TOK_CONTINUE,
	TOK_SWITCH,
	TOK_CASE,
	TOK_DEFAULT,

	TOK_CLASS,
	TOK_PRIVATE,
	TOK_PROTECTED,
	TOK_PUBLIC,
	TOK_STATIC,

	TOK_NEW,
	TOK_DELETE,

	// data types
	TOK_INT,
	TOK_FLOAT,
	TOK_DOUBLE,
	TOK_CHAR,
	TOK_STRING,
	TOK_BOOL,
	TOK_VOID,

	// operators arithmetic
	TOK_PLUS,             // +
	TOK_MINUS,            // -
	TOK_MULTIPLY,         // *
	TOK_DIVIDE,           // /
	TOK_MODULO,           // %

	// operators inc/dec
	TOK_INCREMENT,        // ++
	TOK_DECREMENT,        // --

	// operators relational
	TOK_EQUAL,            // ==
	TOK_NOT_EQUAL,        // !=
	TOK_LESS_THAN,        // <
	TOK_GREATER_THAN,     // >
	TOK_LESS_EQUAL,       // <=
	TOK_GREATER_EQUAL,    // >=

	// operators logical
	TOK_AND,              // &&
	TOK_OR,               // ||
	TOK_NOT,              // !

	// operators assign
	TOK_ASSIGN,           // =
	TOK_PLUS_ASSIGN,      // +=
	TOK_MINUS_ASSIGN,     // -=
	TOK_MULTIPLY_ASSIGN,  // *=
	
	// operator misc
	TOK_MEMBER_ACCESS,    // .  as in class.member
	TOK_POINTER_ACCESS,   // -> as in classInstance->member
	TOK_SCOPE,            // :: as in stuff like namespace::class::member

	// punctuation
	TOK_SEMICOLON,        // ;
	TOK_COLON,            // :
	TOK_DOT,              // .
	TOK_COMMA,            // ,
	TOK_OPEN_PAR,         // (
	TOK_CLOSE_PAR,        // )
	TOK_OPEN_BRACE,       // {
	TOK_CLOSE_BRACE,      // }
	TOK_OPEN_BRACKET,     // [
	TOK_CLOSE_BRACKET,    // ]

	// identifier
	TOK_IDENTIFIER,

	// literals
	TOK_INTEGER_LIT,
	TOK_FLOAT_LIT,
	TOK_CHAR_LIT,
	TOK_STRING_LIT,
	TOK_BOOLEAN_LIT,
	TOK_NULL_LIT,

	// error handling
	TOK_ERROR
};

struct Token
{
	token_kind kind = token_kind::TOK_EOF;
	std::optional<std::string> val = std::nullopt;
	size_t line = 0;
	size_t col = 0;
};