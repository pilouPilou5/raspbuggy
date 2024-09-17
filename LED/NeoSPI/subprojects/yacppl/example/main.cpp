#include <Logger.hpp>
#include <yaml-cpp/yaml.h>

int main() {
	Logger::begin(YAML::Load("log_level: 4\n" // Begin is not strictly required: if not called it will assume some default settings
				 "use_line_numbers: true\n"
				 "use_light_mode_colors: false\n"
				 "log_aereas:\n"
				 "  - ALL"));

	Logger::debug("Hello");
	Logger::error("This is a error");
	Logger::warn("This is a warning");
	Logger::info("This is a info coming from log aerea 'TEST'", "TEST");
	Logger::urgent("First line\nSecond line", "TWO_LINES");
}
