#pragma once

#include <iostream>
#include <cctype>

namespace amanises
{
	class Utils
	{
	public:
		static bool isAlpha(char c);
		static bool isAlphanum(char c);
		static bool isNum(char c);
		static bool isWhitespace(char c);
		static bool isBoundaryCharacter(char c);
	};
}