#include "quad.h"

#include "mesh.h"

#include "gl_texture.h"

#include "material.h"
#include "utils.h"

namespace Engine {
	Quad::Quad() : Object(ObjectType::QUAD) {
		load_mesh();
	}

	void Quad::load_mesh() {
		std::vector<Utils::Vertex> vertices = {
			Utils::Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),    //bottom left
			Utils::Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),    //bottom right
			Utils::Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),    //top left
			Utils::Vertex(glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f))     //top right
		};

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 1, 3
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
