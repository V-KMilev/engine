#include "model.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "gl_texture.h"

#include "logger.h"

#include "mesh.h"

namespace Engine {
	Model::Model(const std::string& path) : _mPath(path), Object(ObjectType::MODEL) {
		load_mesh();
	}

	const std::string& Model::getPath() const {
		return _mPath;
	}

	std::string& Model::getPath() {
		return _mPath;
	}

	void Model::load_mesh() {
		if (!load_data()) {
			return;
		}
	}

	bool Model::load_data() {
		tinyobj::ObjReader reader;
		tinyobj::ObjReaderConfig reader_config;
		// reader_config.mtl_search_path = "";

		if (!reader.ParseFromFile(_mPath, reader_config)) {
			if (!reader.Error().empty()) {
				LOG(reader.Error().c_str(), LOG_LEVEL::L_ERROR);
			}
			return {};
		}

		if (!reader.Warning().empty()) {
			LOG(reader.Warning().c_str(), LOG_LEVEL::L_WARN);
		}

		const tinyobj::attrib_t& attributes               = reader.GetAttrib();
		const std::vector<tinyobj::shape_t>& shapes       = reader.GetShapes();
		const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

		data_fill(&attributes, &shapes, &materials);

		return true;
	}

	void Model::data_fill(
		const tinyobj::attrib_t* attributes,
		const std::vector<tinyobj::shape_t>* shapes,
		const std::vector<tinyobj::material_t>* materials
	) {
		// Loop over shapes
		for (size_t s = 0; s < shapes->size(); s++) {
			std::vector<Utils::Vertex> vertices = {};
			std::vector<unsigned int> indices = {};
			std::vector<std::shared_ptr<Core::Texture>> textures = {};

			size_t index_offset = 0;

			// Loop over faces(polygon)
			for (size_t f = 0; f < shapes[0][s].mesh.num_face_vertices.size(); f++) {
				size_t fv = size_t(shapes[0][s].mesh.num_face_vertices[f]);

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					tinyobj::index_t idx = shapes[0][s].mesh.indices[index_offset + v];

					glm::vec3 position(0.0f, 0.0f, 0.0f);
					glm::vec3 normal(0.0f, 0.0f, 0.0f);
					glm::vec2 textCoord(0.0f, 0.0f);

					position[0] = attributes->vertices[3 * size_t(idx.vertex_index) + 0];
					position[1] = attributes->vertices[3 * size_t(idx.vertex_index) + 1];
					position[2] = attributes->vertices[3 * size_t(idx.vertex_index) + 2];

					if (idx.normal_index >= 0) {
						normal[0] = attributes->normals[3 * size_t(idx.normal_index) + 0];
						normal[1] = attributes->normals[3 * size_t(idx.normal_index) + 1];
						normal[2] = attributes->normals[3 * size_t(idx.normal_index) + 2];
					}

					if (idx.texcoord_index >= 0) {
						textCoord[0] = attributes->texcoords[2 * size_t(idx.texcoord_index) + 0];
						textCoord[1] = attributes->texcoords[2 * size_t(idx.texcoord_index) + 1];
					}

					vertices.push_back({position, normal, textCoord});
					indices.push_back((unsigned int)idx.vertex_index);
				}
				index_offset += fv;

				// Retrieve material index for the current face
				int materialID = shapes[0][s].mesh.material_ids[f];

				textures.emplace_back(material_fill(materials, materialID));
			}

			push_mesh(vertices, indices, std::move(textures));
		}
	}

	std::shared_ptr<Core::Texture> && Model::material_fill(const std::vector<tinyobj::material_t>* materials, int materialID) {
		// Default texture
		std::string texturePath = "../asset/textures/default/tmp.jpg";
		
		// Check if the material index is valid
		if (materialID >= 0 && static_cast<size_t>(materialID) < materials->size()) {
			const tinyobj::material_t& material = materials[0][materialID];

			// Load the material data
			_mRenderData.ambient       = glm::make_vec3(material.ambient);
			_mRenderData.diffuse       = glm::make_vec3(material.diffuse);
			_mRenderData.specular      = glm::make_vec3(material.specular);
			_mRenderData.transmittance = glm::make_vec3(material.transmittance);
			_mRenderData.emission      = glm::make_vec3(material.emission);

			_mRenderData.shininess = material.shininess;
			_mRenderData.ior       = material.ior;

			_mRenderData.roughness = material.roughness;
			_mRenderData.metallic  = material.metallic;
			_mRenderData.sheen     = material.sheen;

			// Load the material texture
			texturePath = material.ambient_texname.empty()            ? texturePath : material.ambient_texname;
			texturePath = material.diffuse_texname.empty()            ? texturePath : material.diffuse_texname;
			texturePath = material.specular_texname.empty()           ? texturePath : material.specular_texname;
			texturePath = material.specular_highlight_texname.empty() ? texturePath : material.specular_highlight_texname;
			texturePath = material.bump_texname.empty()               ? texturePath : material.bump_texname;
			texturePath = material.displacement_texname.empty()       ? texturePath : material.displacement_texname;
			texturePath = material.alpha_texname.empty()              ? texturePath : material.alpha_texname;
			texturePath = material.reflection_texname.empty()         ? texturePath : material.reflection_texname;

			texturePath = material.roughness_texname.empty() ? texturePath : material.roughness_texname;
			texturePath = material.metallic_texname.empty()  ? texturePath : material.metallic_texname;
			texturePath = material.sheen_texname.empty()     ? texturePath : material.sheen_texname;
			texturePath = material.emissive_texname.empty()  ? texturePath : material.emissive_texname;
			texturePath = material.normal_texname.empty()    ? texturePath : material.normal_texname;
		}

		return std::move(std::make_shared<Core::Texture>(texturePath));
	}

	void Model::push_mesh(
		const std::vector<Utils::Vertex>& vertices,
		const std::vector<unsigned int>& indices,
		std::vector<std::shared_ptr<Core::Texture>> && textures
	) {
		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices, std::move(textures)));
	}
};
