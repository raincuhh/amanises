#include "error.hpp"

amanises::CompilerError::CompilerError(ErrorCode code, const std::string& msg) :
	mMessage(msg),
	mErrorCode(code) {
}

amanises::SyntaxError::SyntaxError(const std::string& msg, int line) :
	CompilerError(ErrorCode::SYNTAX_ERROR, msg),
	mLine(line) {
}

std::string amanises::SyntaxError::getDetailedMessage() const {
	return "Syntax Error at line " + std::to_string(mLine) + ": " + CompilerError::getMessage();
}

amanises::SemanticError::SemanticError(const std::string& msg, const std::string& context) :
	CompilerError(ErrorCode::SEMANTIC_ERROR, msg),
	mErrorContext(context) {
}

std::string amanises::SemanticError::getDetailedMessage() const {
	return "Semantic Error in context '" + mErrorContext + "': " + CompilerError::getMessage();
}