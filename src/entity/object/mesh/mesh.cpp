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

namespace Engine {
	Mesh::Mesh(
			const std::vector<Utils::Vertex>& vertices,
			const std::vector<unsigned int>& indices,
			std::vector<std::shared_ptr<Core::Texture>> && textures
	) : _mTexture(std::move(textures)), Entity(EntityType::MESH) {

		M_ASSERT(!vertices.empty() && !indices.empty());

		for(std::shared_ptr<Core::Texture>& texture : _mTexture) {
			texture->init();
		}

		_mVB = std::make_shared<Core::VertexBuffer>(&vertices[0], vertices.size() * sizeof(Utils::Vertex));
		_mIB = std::make_shared<Core::IndexBuffer>(&indices[0], indices.size());
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

		updateModel();
	}

	const MeshWorldData& Mesh::getWorldData() const {
		return _mMeshWorldData;
	}

	MeshWorldData& Mesh::getWorldData() {
		return _mMeshWorldData;
	}

	const MeshUseData& Mesh::getUseData() const {
		return _mMeshUseData;
	}

	MeshUseData& Mesh::getUseData() {
		return _mMeshUseData;
	}

	void Mesh::onUpdate(const Mouse* mouse, float deltaTime) {
		auto& ud = _mMeshUseData;

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

		for(const std::shared_ptr<Core::Texture>& texture : _mTexture) {
			texture->bind(texture->getID());
			shader.setUniform1ui(texture->getName(), texture->getID());
		}

		shader.setUniformMatrix4fv("uModel", _mMeshWorldData.model);
	}

	void Mesh::updateModel(glm::mat4 objectModel) {
		auto& wd = _mMeshWorldData;

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
		auto& ud = _mMeshUseData;
		auto& wd = _mMeshWorldData;

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

	void Mesh::drawUiTextures() {
		for(const std::shared_ptr<Core::Texture>& texture : _mTexture) {
			// std::string sTexture = "Texture: #" + std::to_string(texture->getID());

			// ImGui::SeparatorText(sTexture.c_str());

			// ImGui::BulletText(texture->getName().c_str());
		}
	}
};
