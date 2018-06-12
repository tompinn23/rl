#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "u-util.hpp"

int main(int argc, char* argv[]) {
	// global setup...
	init_logging();
	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	return result;
}
