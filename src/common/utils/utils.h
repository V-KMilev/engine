#pragma once

#include <iomanip>
#include <sstream>
#include <chrono>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

namespace Engine {
	namespace Utils {
		struct Vertex {
			public:
				Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) :
					position(position),
					normal(normal),
					texCoords(texCoords) {}

			public:
				glm::vec3 position;
				glm::vec3 normal;
				// glm::vec3 tangent;
				glm::vec2 texCoords;
		};

		static std::string get_current_time(const std::string& format = "%Y.%m.%d %H:%M:%S") {
			const std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			std::stringstream ss;
			ss << std::put_time(localtime(&current_time), format.c_str());

			return ss.str();
		}
	};
};
