#include <memory>
#include "driver.hpp"

int main(int argc, char* argv[])
{
	std::unique_ptr<amanises::Driver> driver = std::make_unique<amanises::Driver>();
	return driver->driver_init(argc, argv);
}