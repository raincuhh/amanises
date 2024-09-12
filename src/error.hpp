#pragma once

#include <exception>
#include <iostream>
#include <memory>
#include <string>

enum class ErrorCode {
	SUCCESS = 0,
	SYNTAX_ERROR,
	SEMANTIC_ERROR,
	FILE_NOT_FOUND,
	OUT_OF_MEMORY,
	UNKNOWN_ERROR
};

enum class ErrorType {
	COMPILER_ERROR = 0,
	SYNTAX_ERROR,
	SEMANTIC_ERROR
};

namespace amanises {
	class CompilerError {
	public:
		CompilerError(ErrorCode code, const std::string& msg);
		virtual ~CompilerError() = default;


		ErrorCode getErrorCode() const { return mErrorCode; };
		std::string getMessage() const { return mMessage; }
		
	private:
		std::string mMessage;
		ErrorCode mErrorCode;
	};

	class SyntaxError : public CompilerError {
	public:
		SyntaxError(const std::string& msg, int line);

		int getLine() const { return mLine; };
		std::string getDetailedMessage() const;

	private:
		int mLine;
	};

	class SemanticError : public CompilerError {
	public:
		SemanticError(const std::string& msg, const std::string& context);

		std::string getContext() const { return mErrorContext; };
		std::string getDetailedMessage() const;

	private:
		std::string mErrorContext;
	};
}

