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

#include "error_handle.h"

namespace Engine {
	Mesh::Mesh(
		const std::vector<Utils::Vertex>& mVertices,
		const std::vector<unsigned int>&  mIndices,
		const std::vector<unsigned int>&  mTextureIDs
	) : Entity(EntityType::MESH) {

		M_ASSERT(!mVertices.empty() && !mIndices.empty());

		_mVB = std::make_shared<Core::VertexBuffer>(&mVertices[0], mVertices.size() * sizeof(Utils::Vertex));
		_mIB = std::make_shared<Core::IndexBuffer>(&mIndices[0], mIndices.size());
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
			// updateModel();

			ud.hasUpdate = false;
		}
	}

	void Mesh::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		updateShader(shader);

		renderer.draw(*_mVA, *_mIB, shader);
	}

	void Mesh::drawUIParams() {
		auto& ud = _mMeshUseData;
		auto& wd = _mMeshWorldData;

		ImGui::BulletText("Mesh:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.0f), "%u", _mID.getID());

		std::string position = "Position##Mesh" + std::to_string(_mID.getID());
		std::string rotation = "Rotation##Mesh" + std::to_string(_mID.getID());
		std::string scale    = "Scale##Mesh"    + std::to_string(_mID.getID());

		if(ImGui::DragFloat3(position.c_str(), &wd.position[0], 1)) {
			ud.hasUpdate = true;
		}
		if(ImGui::DragFloat3(rotation.c_str(), &wd.rotation[0], 1)) {
			ud.hasUpdate = true;
		}
		if(ImGui::DragFloat3(scale.c_str(), &wd.scale[0], 1)) {
			ud.hasUpdate = true;
		}
	}

	void Mesh::updateShader(const Core::Shader &shader) const {
		shader.bind();

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
};
