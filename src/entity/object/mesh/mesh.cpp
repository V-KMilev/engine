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
		// TODO: Check if this is correct
		_mMaterial = material;
		load_mesh();
	}

	void Mesh::load_mesh() {
		M_ASSERT(!_mVertices->empty() && !_mIndices->empty());

		for (std::shared_ptr<Core::Texture>& texture : _mMaterial->getTextures()->textures) {
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
		// TODO: Think of what we can do in this situation
		if (_mUseData->hasUpdate) {

			// Reset the update event
			_mUseData->hasUpdate = false;
		}
	}

	void Mesh::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		updateShader(shader);

		renderer.draw(*_mVA, *_mIB, shader);
	}

	void Mesh::updateShader(const Core::Shader &shader) const {
		auto worldData = static_cast<ObjectWorldData*>(_mWorldData.get());

		shader.bind();

		_mMaterial->updateShader(shader);

		shader.setUniformMatrix4fv("uModel", worldData->model);
	}

	void Mesh::updateModel(glm::mat4 objectModel) {
		auto worldData = static_cast<ObjectWorldData*>(_mWorldData.get());

		worldData->model = glm::mat4(1.0f);

		worldData->model = glm::translate(worldData->model, worldData->position);

		worldData->model = glm::rotate(worldData->model, glm::radians(worldData->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		worldData->model = glm::rotate(worldData->model, glm::radians(worldData->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		worldData->model = glm::rotate(worldData->model, glm::radians(worldData->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		worldData->model = glm::scale(worldData->model, worldData->scale);

		// TODO: Think if we can fix the "problem"
		worldData->model *= objectModel;
	}

	void Mesh::UIWorld() {
		std::string sMesh = "Mesh: #" + std::to_string(_mID.getID());

		if (ImGui::TreeNode(sMesh.c_str())) {

			_mUseData->drawUI(_mID.getID());

			ImGui::SeparatorText("World Data");

			_mWorldData->drawUI(_mID.getID());

			ImGui::TreePop();
		}
	}

	void Mesh::UIMaterialTextures() {
		_mMaterial->UITextures(_mID.getID());
	}

	void Mesh::UIMaterialCoefficients() {
		_mMaterial->UICoefficients(_mID.getID());
	}
};
