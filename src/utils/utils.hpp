#pragma once

#include <iostream>
#include <cctype>

namespace amanises
{
	class Utils
	{
	public:
		static inline bool isAlpha(char c);
		static inline bool isAlphanum(char c);
		static inline bool isNum(char c);
		static inline bool isWhitespace(char c);
	};
}