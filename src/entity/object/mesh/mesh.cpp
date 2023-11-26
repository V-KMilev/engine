#include "mesh.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_vertex_array.h"
#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"
#include "gl_vertex_buffer_layout.h"

#include "gl_render.h"
#include "gl_shader.h"
#include "gl_texture.h"

#include "error_handle.h"

#include "material.h"
#include "utils.h"

namespace Engine {
	Mesh::Mesh(
			const std::vector<Utils::Vertex>& vertices,
			const std::vector<unsigned int>& indices,
			std::shared_ptr<Material> material

	) : Object(ObjectType::MESH),
		_mVertices(new std::vector<Utils::Vertex>(vertices)),
		_mIndices(new std::vector<unsigned int>(indices))
	{
			_mMaterial = material;
			load_mesh();
	}

	void Mesh::load_mesh() {
		M_ASSERT(!_mVertices->empty() && !_mIndices->empty());

		for(std::shared_ptr<Core::Texture>& texture : _mMaterial->getTextures().textures) {
			texture->init();
		}

		_mVB = std::make_shared<Core::VertexBuffer>(_mVertices->data(), _mVertices->size() * sizeof(Utils::Vertex));
		_mIB = std::make_shared<Core::IndexBuffer>(_mIndices->data(), _mIndices->size());
		_mVA = std::make_shared<Core::VertexArray>();

		_mVBL = std::make_shared<Core::VertexBufferLayout>();
		// The layout is the same as the struct of the Vertex:
		// float float float - position
		// float float float - normal
		// float float       - texCoords
		_mVBL->push<float>(3);
		_mVBL->push<float>(3);
		_mVBL->push<float>(2);

		_mVA->addBuffer(*_mVB, *_mVBL);

		// Clean the cashe data
		_mVertices->clear();
		_mVertices->shrink_to_fit();
		_mIndices->clear();
		_mIndices->shrink_to_fit();

		updateModel();
	}

	void Mesh::onUpdate(const Mouse* mouse, float deltaTime) {
		auto& ud = _mObjectUseData;

		// TODO: Think of what we can do in this situation
		if (ud.hasUpdate) {

			// Reset the update event
			ud.hasUpdate = false;
		}
	}

	void Mesh::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		updateShader(shader);

		renderer.draw(*_mVA, *_mIB, shader);
	}

	void Mesh::updateShader(const Core::Shader &shader) const {
		shader.bind();
		
		_mMaterial->updateShader(shader);

		shader.setUniformMatrix4fv("uModel", _mObjectWorldData.model);
	}

	void Mesh::updateModel(glm::mat4 objectModel) {
		auto& wd = _mObjectWorldData;

		wd.model = glm::mat4(1.0f);

		wd.model = glm::translate(wd.model, wd.position);

		wd.model = glm::rotate(wd.model, glm::radians(wd.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		wd.model = glm::rotate(wd.model, glm::radians(wd.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		wd.model = glm::rotate(wd.model, glm::radians(wd.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		wd.model = glm::scale(wd.model, wd.scale);

		// TODO: Think if we can fix the "problem"
		wd.model *= objectModel;
	}

	void Mesh::drawUIParams() {
		auto& ud = _mObjectUseData;
		auto& wd = _mObjectWorldData;

		std::string sMesh = "Mesh: #" + std::to_string(_mID.getID());

		std::string sPosition = "Position##Mesh" + std::to_string(_mID.getID());
		std::string sRotation = "Rotation##Mesh" + std::to_string(_mID.getID());
		std::string sScale    = "Scale##Mesh"    + std::to_string(_mID.getID());

		if(ImGui::TreeNode(sMesh.c_str())) {

			ImGui::SeparatorText(sMesh.c_str());

			if(ImGui::DragFloat3(sPosition.c_str(), &wd.position[0], 1)) { ud.hasUpdate = true; }
			if(ImGui::DragFloat3(sRotation.c_str(), &wd.rotation[0], 1)) { ud.hasUpdate = true; }
			if(ImGui::DragFloat3(sScale.c_str(),    &wd.scale[0],    1)) { ud.hasUpdate = true; }

			ImGui::TreePop();
		}
	}
};
