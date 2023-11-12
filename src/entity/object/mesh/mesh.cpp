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

#include "utils.h"

namespace Engine {
	Mesh::Mesh(
			const std::vector<Utils::Vertex>& vertices,
			const std::vector<unsigned int>& indices,
			std::vector<std::shared_ptr<Core::Texture>> && textures
	) : _mTextures(std::move(textures)), Entity(EntityType::MESH) {

		M_ASSERT(!vertices.empty() && !indices.empty());

		for(std::shared_ptr<Core::Texture>& texture : _mTextures) {
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

		for(int idx = 0; idx < _mTextures.size(); idx++) {
			const std::shared_ptr<Core::Texture>& texture = _mTextures[idx];

			texture->bind(idx);

			std::string material = "uMaterial." + texture->getShaderName();

			shader.setUniform1i(material, idx);
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
		std::string sMesh = "Mesh: #" + std::to_string(_mID.getID());

		if(ImGui::TreeNode(sMesh.c_str())) {
			static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersH | ImGuiTableFlags_RowBg;

			ImGui::Unindent();

			if (ImGui::BeginTable("table1", 3, tableFlags)) {
				ImGui::TableSetupColumn("Texture Tpye:");
				ImGui::TableSetupColumn("Texture:");
				ImGui::TableSetupColumn("Data:");

				ImGui::TableHeadersRow();

				for(const std::shared_ptr<Core::Texture>& texture : _mTextures) {
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::NewLine();
					ImGui::NewLine();
					ImGui::Indent();
					ImGui::TextColored(ImVec4(0.96f, 0.26f, 0.26f, 1.00f), "%s", texture->getShaderName().c_str());
					ImGui::Unindent();

					ImGui::TableSetColumnIndex(1);
					ImGui::Image((ImTextureID)texture->getID(), ImVec2(100.0f, 100.0f));

					ImGui::TableSetColumnIndex(2);

					Utils::UI::ColoredBulletText("ID:", std::to_string(texture->getID()), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Width:", std::to_string(texture->getWidth()), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Height:", std::to_string(texture->getHeight()), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Path:", texture->getPath(), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Name:", texture->getName(), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));

					if (ImGui::TreeNodeEx("OpenGL Data", ImGuiTreeNodeFlags_Selected)) {
						const auto& tParams = texture->getParams();

						Utils::UI::ColoredBulletText("Border:", std::to_string(tParams.border), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
						Utils::UI::ColoredBulletText("Target:", std::to_string(tParams.target), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
						Utils::UI::ColoredBulletText("Level:", std::to_string(tParams.level), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
						Utils::UI::ColoredBulletText("Internal Format:", std::to_string(tParams.internalFormat), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
						Utils::UI::ColoredBulletText("Border:", std::to_string(tParams.border), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
						Utils::UI::ColoredBulletText("Format:", std::to_string(tParams.format), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
						Utils::UI::ColoredBulletText("Type:", std::to_string(tParams.type), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));

						ImGui::TreePop();
					}
				}
				ImGui::EndTable();
			}
			ImGui::TreePop();
		}
	}
};
