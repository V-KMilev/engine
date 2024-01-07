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

		static std::vector<glm::vec3> getNormals(const std::vector<Utils::Vertex>& vertices, const std::vector<unsigned int>& indices) {
			std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f));

			// Calculate face normals and accumulate them for each vertex
			for (size_t idx = 0; idx < indices.size(); idx += 3) {
				glm::vec3 v1 = vertices[indices[idx]].position;
				glm::vec3 v2 = vertices[indices[idx + 1]].position;
				glm::vec3 v3 = vertices[indices[idx + 2]].position;

				glm::vec3 a = v2 - v1;
				glm::vec3 b = v3 - v1;

				glm::vec3 normal = glm::cross(a, b);

				normals[indices[idx]]     += normal;
				normals[indices[idx + 1]] += normal;
				normals[indices[idx + 2]] += normal;
			}

			// Normalize the accumulated normals
			for(int idx = 0; idx < normals.size(); idx++) {
				normals[idx] = glm::normalize(normals[idx]);
			}

			return normals;
		}

		static std::string get_current_time(const std::string& format = "%Y.%m.%d %H:%M:%S") {
			const std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			std::stringstream ss;
			ss << std::put_time(localtime(&current_time), format.c_str());

			return ss.str();
		}

		namespace UI {
			static void ColoredBulletText(const std::string& bulletText, const std::string& coloredText, ImVec4 color) {
				ImGui::BulletText("%s", bulletText.c_str());
				ImGui::SameLine();
				ImGui::TextColored(color, "%s", coloredText.c_str());
			}
		};
	};
};
