#include "model.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/rotate_vector.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "gl_texture.h"

#include "logger.h"

#include "material.h"

#include "mesh.h"

#include "utils.h"

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

			std::shared_ptr<Material> material = load_material(materials);

			_mMeshes.push_back(std::make_shared<Mesh>(vertices, indices, material));
		}
	}

	std::shared_ptr<Material> Model::load_material(const std::vector<tinyobj::material_t>* materials) {
		std::shared_ptr<Material> my_material = std::make_shared<Material>();

		std::string rootPath = _mPath.substr(0, _mPath.find_last_of('/'));

		// Load the material data
		for (const tinyobj::material_t& material : materials[0]) {
			my_material->setAmbient(glm::make_vec3(material.ambient));
			my_material->setDiffuse(glm::make_vec3(material.diffuse));
			my_material->setSpecular(glm::make_vec3(material.specular));
			my_material->setTransmittance(glm::make_vec3(material.transmittance));
			my_material->setEmission(glm::make_vec3(material.emission));

			my_material->setShininess(material.shininess);
			my_material->setIor(material.ior);

			my_material->setRoughness(material.roughness);
			my_material->setMetallic(material.metallic);
			my_material->setSheen(material.sheen);

			std::vector<std::string> texnames = {
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

			for (int idx = 0; idx < texnames.size(); idx++) {
				if (texnames[idx] != "") {
					my_material->getTextures()[idx] = std::make_shared<Core::Texture>(rootPath + "/" + texnames[idx]);
				}
			}
		}

		return my_material;
	}
};
