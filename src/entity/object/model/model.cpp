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
					indices.push_back(vertices.size() - 1);
				}
				index_offset += fv;
			}

			load_materials(materials, textures);

			push_mesh(vertices, indices, std::move(textures));
		}
	}

	void Model::load_materials(
		const std::vector<tinyobj::material_t>* materials,
		std::vector<std::shared_ptr<Core::Texture>>& textures
	) {
		// Default texture
		std::string defaultPath = "../asset/textures/default/texture.png";

		std::vector<std::string> texturePaths(13, "");

		for(const tinyobj::material_t& material : materials[0]) {
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

			std::vector<std::string> newTexturePaths = {
				material.ambient_texname,
				material.diffuse_texname,
				material.specular_texname,
				material.specular_highlight_texname,
				material.bump_texname,
				material.displacement_texname,
				material.alpha_texname,
				material.reflection_texname,
				material.roughness_texname,
				material.metallic_texname,
				material.sheen_texname,
				material.emissive_texname,
				material.normal_texname
			};

			for (int idx = 0; idx < texturePaths.size(); idx++) {
				if (texturePaths[idx].empty() && !newTexturePaths[idx].empty()) {
					texturePaths[idx] = newTexturePaths[idx];
				}
			}
		}

		for (int idx = 0; idx < texturePaths.size(); idx++) {
			std::string texturePath = defaultPath;
			if(!texturePaths[idx].empty()) {
				texturePath = _mPath.substr(0, _mPath.find_last_of('/')) + "/" + texturePaths[idx];
			}

			textures.emplace_back(std::make_shared<Core::Texture>(texturePath, Core::TextureShaderType(idx + 1)));
		}
	}

	void Model::push_mesh(
		const std::vector<Utils::Vertex>& vertices,
		const std::vector<unsigned int>& indices,
		std::vector<std::shared_ptr<Core::Texture>> && textures
	) {
		_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices, std::move(textures)));
	}
};
