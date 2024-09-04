#pragma once

#include <iostream>
#include <string>
#include <optional>

enum class TokenType {

	// reserved keywords
	IF,
	ELSE,
	FOR,
	WHILE,
	RETURN,
	BREAK,
	CONTINUE,
	SWITCH,
	CASE,
	DEFAULT,

	CLASS,
	PRIVATE,
	PROTECTED,
	PUBLIC,
	STATIC,

	NEW,
	DELETE,

	// data types
	INT,
	FLOAT,
	DOUBLE,
	CHAR,
	STRING,
	BOOL,
	VOID,

	// operators
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	ASSIGN,
	EQUAL,
	NOT_EQUAL,
	LESS_THAN,
	GREATER_THAN,
	LESS_EQUAL,
	GREATER_EQUAL,
	AND,
	OR,
	NOT,
	INCREMENT,
	DECREMENT,

	// punctuation
	SEMICOLON,        // ;
	COLON,            // :
	DOT,              // .
	COMMA,            // ,
	OPEN_PAR,         // (
	CLOSE_PAR,        // )
	OPEN_BRACE,       // {
	CLOSE_BRACE,      // }
	OPEN_BRACKET,     // [
	CLOSE_BRACKET,    // ]

	// identifier and literals
	IDENTIFIER,       
	INTEGER_LIT,
	FLOAT_LIT,
	CHAR_LIT,
	STRING_LIT,
	BOOLEAN_LIT,
	NULL_LIT,

	// comments
	LINE_COMMENT,
	BLOCK_COMMENT,

	// function related
	FUNCTION,         // a function definition
	METHOD,           // a method inside a class
	RETURN_TYPE,      // return type of a function

	//special
	PRAGMA,
	IMPORT,
	_EOF,
	_SOF,

	// error handling
	ERROR
};

struct Token
{
	TokenType type;
	std::optional<std::string> value {};
};