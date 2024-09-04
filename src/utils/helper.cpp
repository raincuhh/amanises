#include "helper.hpp"

inline bool amanises::Helper::isAlpha(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

inline bool amanises::Helper::isAlphanum(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

inline bool amanises::Helper::isNum(char c)
{
	return false;
}
