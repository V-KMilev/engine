#include "sphere.h"

#include "mesh.h"

#include "gl_texture.h"

#include "material.h"
#include "utils.h"

namespace Engine {
	Sphere::Sphere() : Object(ObjectType::SPHERE) {
		load_mesh();
	}

	void Sphere::load_mesh() {
		std::vector<Utils::Vertex> vertices;
		std::vector<unsigned int> indices;

		int stacks = 25;
		int slices = 25;

		float radius = 1.0f;

		for (int i = 0; i <= stacks; i++) {
			float phi = static_cast<float>(i) / stacks * glm::pi<float>();

			for (int j = 0; j <= slices; j++) {
				float theta = static_cast<float>(j) / slices * 2.0f * glm::pi<float>();

				float x = std::sin(phi) * std::cos(theta);
				float y = std::cos(phi);
				float z = std::sin(phi) * std::sin(theta);

				glm::vec3 position = radius * glm::vec3(x, y, z);
				glm::vec3 normal = glm::normalize(position);
				glm::vec2 uv(static_cast<float>(j) / slices, static_cast<float>(i) / stacks);

				vertices.emplace_back(Utils::Vertex(position, normal, uv));
			}
		}

		for (int i = 0; i < stacks; i++) {
			for (int j = 0; j < slices; j++) {
				int p0 = i * (slices + 1) + j;
				int p1 = p0 + slices + 1;

				int p2 = p1 + 1;
				int p3 = p0 + 1;

				indices.push_back(p0);
				indices.push_back(p1);
				indices.push_back(p2);

				indices.push_back(p0);
				indices.push_back(p2);
				indices.push_back(p3);
			}
		}

		// Default texture
		std::string defaultPath = "../asset/textures/default/texture.png";

		for (std::shared_ptr<Core::Texture>& texture : _mMaterial->getTextures()->textures) {
			texture = std::make_shared<Core::Texture>(defaultPath);
		}

		// Now, you can pass these vectors to your functions
		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices, _mMaterial));
	}
};
