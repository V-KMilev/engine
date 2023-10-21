#pragma once

#include <sstream>
#include <chrono>

namespace Engine {
	namespace Utils {

		std::string get_current_time(const std::string& format = "%Y.%m.%d %H:%M:%S") {
			const std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			std::stringstream ss;
			ss << std::put_time(localtime(&current_time), format.c_str());

			return ss.str();
		}
	};
};
