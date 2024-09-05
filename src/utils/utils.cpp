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