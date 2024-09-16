#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include <system_error>

#include "logger.hpp"
#include "compilerPipeline.hpp"
#include "logger.hpp"

namespace amanises {
	class Driver {
	public:
		using CompilerPipeline = amanises::CompilerPipeline;
		using Logger = amanises::Logger;
		Driver();
		bool driver_init(int argc, char* argv[]);
	private:

		static bool validate_usage_arguments(int argc, char* argv[], Logger* m_logger);
		static bool check_working_directory(Logger* m_logger);
		static bool compiler_pipeline_init(int argc, char* argv[], Logger* m_logger);
		static void usage();
	};
}