#pragma once

#include "../../src/logger.hpp"

namespace amanises
{
	class Parser
	{
	public:
		Parser();
		~Parser();

	private:
		Logger* m_logger;
	};

}

