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

		int stacks = 50;
		int slices = 50;

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

		std::vector<glm::vec3> normals = getNormals(vertices, indices);

		for (int idx = 0; idx < vertices.size(); idx++) {
			// Inverting the normals because its a sphere
			vertices[idx].normal = normals[idx] * glm::vec3(-1.0f);
		}

		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices));
	}
};
