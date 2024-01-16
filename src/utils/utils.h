#pragma once

#include <iomanip>
#include <sstream>
#include <chrono>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "input_manager.h"

#include "gl_render.h"

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

	namespace Time {
		static std::string get_current_time(const std::string& format = "%Y.%m.%d %H:%M:%S") {
			const std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			std::stringstream ss;
			ss << std::put_time(localtime(&current_time), format.c_str());

			return ss.str();
		}

		extern float deltaTime;
		extern int FPS;
		extern int frame;
	};

	namespace IO {
		extern Engine::InputManager inputManager;
	};

	namespace UI {
		static void ColoredBulletText(const std::string& bulletText, const std::string& coloredText, ImVec4 color) {
			ImGui::BulletText("%s", bulletText.c_str());
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", coloredText.c_str());
		}
	};

	namespace Render {
		extern Core::Renderer renderer;
	}
};
