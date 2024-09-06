#include "utils.hpp"

inline bool amanises::Utils::isAlpha(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

inline bool amanises::Utils::isAlphanum(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

inline bool amanises::Utils::isNum(char c)
{
	return false;
}

inline bool amanises::Utils::isWhitespace(char c)
{
	return (
		c == ' ' || c == '\n' || c == '\t' ||
		c == '\v' || c == '\b' || c == '\r'
	);
}

bool amanises::Utils::isBoundaryCharacter(char c)
{
	// what this is checking is basically if the character that the splitter splits on, 
	// is a boundary character. if it is not a boundary character, 
	// then it continues until it finds a boundary character.
	return c == '\n' || c == '}' || c == '{' || c == ';';
}
