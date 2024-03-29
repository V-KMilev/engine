#include "triangle.h"

#include "mesh.h"

#include "material.h"

#include "utils.h"

namespace Engine {
	Triangle::Triangle() : Object(ObjectType::TRIANGLE) {
		load_mesh();
	}

	void Triangle::load_mesh() {
		std::vector<Utils::Vertex> vertices = {
			Utils::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),    // bottom left
			Utils::Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),    // bottom right
			Utils::Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f))     // top left
		};

		std::vector<unsigned int> indices = {0, 1, 2};

		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices));
	}
};
