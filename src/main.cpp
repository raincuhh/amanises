#include <memory>
#include "driver.hpp"

using Driver = amanises::Driver;

int main(int argc, char* argv[])
{
	std::unique_ptr<Driver> entryPoint = std::make_unique<Driver>();
	bool result = entryPoint->driver_init(argc, argv);
	return result;
}