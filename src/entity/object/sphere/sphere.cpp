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

		const float PI = glm::pi<float>();

		const unsigned int STACKS = 32;
		const unsigned int SLICES = 32;

		const float radius = 1.0f;

		for (unsigned int segmentX = 0; segmentX <= STACKS; segmentX++) {
			float u = (float)segmentX / STACKS;
			float phi = u * 2.0f * PI;

			for (unsigned int segmentY = 0; segmentY <= SLICES; segmentY++) {
				float v = (float)segmentY / SLICES;
				float theta = v * PI;

				float x = std::cos(phi) * std::sin(theta);
				float y = std::cos(theta);
				float z = std::sin(phi) * std::sin(theta);

				glm::vec3 position = radius * glm::vec3(x, y, z);
				glm::vec3 normal   = position;
				glm::vec2 uv       = glm::vec2(u, v);

				vertices.emplace_back(Utils::Vertex(position, normal, uv));
			}
		}

		for (int i = 0; i < STACKS; i++) {
			for (int j = 0; j < SLICES; j++) {
				int p0 = i * (STACKS + 1) + j;
				int p1 = p0 + SLICES + 1;

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

		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices));
	}
};
