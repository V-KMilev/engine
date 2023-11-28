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
			Utils::Vertex(glm::vec3(-1, -1,  1), glm::normalize(glm::vec3(-1, -1,  1)), glm::vec2(0.0f, 0.0f)), //bottom left
			Utils::Vertex(glm::vec3( 1, -1,  1), glm::normalize(glm::vec3( 1, -1,  1)), glm::vec2(1.0f, 0.0f)), //bottom right
			Utils::Vertex(glm::vec3(-1,  1,  1), glm::normalize(glm::vec3(-1,  1,  1)), glm::vec2(0.0f, 1.0f)), //top left
			Utils::Vertex(glm::vec3( 1,  1,  1), glm::normalize(glm::vec3( 1,  1,  1)), glm::vec2(1.0f, 1.0f)), //top right
			Utils::Vertex(glm::vec3(-1, -1, -1), glm::normalize(glm::vec3(-1, -1, -1)), glm::vec2(0.0f, 0.0f)), //bottom left
			Utils::Vertex(glm::vec3( 1, -1, -1), glm::normalize(glm::vec3( 1, -1, -1)), glm::vec2(1.0f, 0.0f)), //bottom right
			Utils::Vertex(glm::vec3(-1,  1, -1), glm::normalize(glm::vec3(-1,  1, -1)), glm::vec2(0.0f, 1.0f)), //top left
			Utils::Vertex(glm::vec3( 1,  1, -1), glm::normalize(glm::vec3( 1,  1, -1)), glm::vec2(1.0f, 1.0f))  //top right
		};

		std::vector<unsigned int> indices = {
			//Top
			2, 6, 7,
			2, 3, 7,

			//Bottom
			0, 4, 5,
			0, 1, 5,

			//Left
			0, 2, 6,
			0, 4, 6,

			//Right
			1, 3, 7,
			1, 5, 7,

			//Front
			0, 2, 3,
			0, 1, 3,

			//Back
			4, 6, 7,
			4, 5, 7
		};

		// Default texture
		std::string defaultPath = "../asset/textures/default/texture.png";

		for (std::shared_ptr<Core::Texture>& texture : _mMaterial->getTextures().textures) {
			texture = std::make_shared<Core::Texture>(defaultPath);
		}

		// Now, you can pass these vectors to your functions
		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices, _mMaterial));
	}
};
