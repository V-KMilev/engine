#include "mesh.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "error_handle.h"
#include "tracer.h"

#include "gl_vertex_array.h"
#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"
#include "gl_vertex_buffer_layout.h"

#include "gl_render.h"
#include "gl_shader.h"
#include "gl_texture.h"

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
		PROFILER_BEGIN("Mesh", "Mesh Load");

		M_ASSERT(!_mVertices->empty() && !_mIndices->empty());

		PROFILER_BEGIN("Mesh", "Mesh Texture init");
		for (std::shared_ptr<Core::Texture>& texture : _mMaterial->getTextures()->textures) {
			texture->init();
		}
		PROFILER_END("Mesh", "Mesh Texture init");

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

		PROFILER_BEGIN("Mesh", "Mesh Add Buffer");

		_mVA->addBuffer(*_mVB, *_mVBL);

		PROFILER_END("Mesh", "Mesh Add Buffer");

		// Clean the cashe data
		_mVertices->clear();
		_mVertices->shrink_to_fit();
		_mIndices->clear();
		_mIndices->shrink_to_fit();

		updateModel();

		PROFILER_END("Mesh", "Mesh Load");
	}

	void Mesh::onUpdate(const Mouse* mouse, float deltaTime) {
		// TODO: Think of what we can do in this situation
		if (_mInteractionState->hasUpdate) {
			PROFILER_BEGIN("Mesh", "Mesh Update");

			// Reset the update event
			_mInteractionState->hasUpdate = false;

			PROFILER_END("Mesh", "Mesh Update");
		}
	}

	void Mesh::draw(const Core::Shader &shader) const {
		PROFILER_BEGIN("Mesh", "Mesh Draw");

		updateShader(shader);

		renderer.draw(*_mVA, *_mIB, shader);

		PROFILER_END("Mesh", "Mesh Draw");
	}

	void Mesh::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Mesh", "Mesh Shader Update");

		auto transform = static_cast<ObjectTransform*>(_mTransform.get());

		shader.bind();

		// TODO: Fix the material

		_mMaterial->updateShader(shader);

		shader.setUniformMatrix4fv("uModel", transform->model);

		PROFILER_END("Mesh", "Mesh Shader Update");
	}

	void Mesh::updateModel(glm::mat4 objectModel) {
		PROFILER_BEGIN("Mesh", "Mesh Update Model");

		auto transform = static_cast<ObjectTransform*>(_mTransform.get());

		transform->model = glm::mat4(1.0f);

		transform->model = glm::translate(transform->model, transform->position);

		transform->model = glm::rotate(transform->model, glm::radians(transform->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		transform->model = glm::rotate(transform->model, glm::radians(transform->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		transform->model = glm::rotate(transform->model, glm::radians(transform->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		transform->model = glm::scale(transform->model, transform->scale);

		// TODO: Think if we can fix the "problem"
		transform->model *= objectModel;

		PROFILER_END("Mesh", "Mesh Update Model");
	}

	void Mesh::UIWorld() {
		std::string sMesh = "Mesh: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sMesh.c_str());

		_mInteractionState->drawUI(_mID.getID());
		_mTransform->drawUI(_mID.getID());
	}

	void Mesh::UIMaterialTextures() {
		_mMaterial->UITextures(_mID.getID());
	}

	void Mesh::UIMaterialCoefficients() {
		_mMaterial->UICoefficients(_mID.getID());
	}
};
