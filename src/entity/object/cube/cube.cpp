#include "cube.h"

#include "mesh.h"

#include "gl_texture.h"

#include "material.h"
#include "utils.h"

namespace Engine {
	Cube::Cube() : Object(ObjectType::QUAD) {
		load_mesh();
	}

	void Cube::load_mesh() {
		std::vector<Utils::Vertex> vertices = {
			Utils::Vertex(glm::vec3(-1.0f, -1.0f,  1.0f), glm::normalize(glm::vec3(-1.0f, -1.0f,  1.0f)), glm::vec2(0.0f, 0.0f)), // Bottom left
			Utils::Vertex(glm::vec3( 1.0f, -1.0f,  1.0f), glm::normalize(glm::vec3( 1.0f, -1.0f,  1.0f)), glm::vec2(1.0f, 0.0f)), // Bottom right
			Utils::Vertex(glm::vec3(-1.0f,  1.0f,  1.0f), glm::normalize(glm::vec3(-1.0f,  1.0f,  1.0f)), glm::vec2(0.0f, 1.0f)), // Top left
			Utils::Vertex(glm::vec3( 1.0f,  1.0f,  1.0f), glm::normalize(glm::vec3( 1.0f,  1.0f,  1.0f)), glm::vec2(1.0f, 1.0f)), // Top right
			Utils::Vertex(glm::vec3(-1.0f, -1.0f, -1.0f), glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), glm::vec2(0.0f, 0.0f)), // Bottom left
			Utils::Vertex(glm::vec3( 1.0f, -1.0f, -1.0f), glm::normalize(glm::vec3( 1.0f, -1.0f, -1.0f)), glm::vec2(1.0f, 0.0f)), // Bottom right
			Utils::Vertex(glm::vec3(-1.0f,  1.0f, -1.0f), glm::normalize(glm::vec3(-1.0f,  1.0f, -1.0f)), glm::vec2(0.0f, 1.0f)), // Top left
			Utils::Vertex(glm::vec3( 1.0f,  1.0f, -1.0f), glm::normalize(glm::vec3( 1.0f,  1.0f, -1.0f)), glm::vec2(1.0f, 1.0f))  // Top right
		};

		std::vector<unsigned int> indices = {
			// Top
			2, 6, 7,
			2, 3, 7,

			// Bottom
			0, 4, 5,
			0, 1, 5,

			// Left
			0, 2, 6,
			0, 4, 6,

			// Right
			1, 3, 7,
			1, 5, 7,

			// Front
			0, 2, 3,
			0, 1, 3,

			// Back
			4, 6, 7,
			4, 5, 7
		};

		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices));
	}
};
