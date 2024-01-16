#include "mesh.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_vertex_array.h"
#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"
#include "gl_vertex_buffer_layout.h"

#include "gl_shader.h"
#include "gl_texture.h"

#include "error_handle.h"
#include "tracer.h"

#include "material.h"

#include "component.h"

namespace Engine {
	Mesh::Mesh(
		const std::vector<Utils::Vertex>& vertices,
		const std::vector<unsigned int>& indices,
		const std::shared_ptr<Material>& material
	) : Entity(EntityType::MESH),
		_mVertices(new std::vector<Utils::Vertex>(vertices)),
		_mIndices(new std::vector<unsigned int>(indices))
	{
		init(material);
	}

	Mesh::Mesh(
		const std::vector<Utils::Vertex>& vertices,
		const std::vector<unsigned int>& indices
	) : Entity(EntityType::MESH),
		_mVertices(new std::vector<Utils::Vertex>(vertices)),
		_mIndices(new std::vector<unsigned int>(indices))
	{
		init(nullptr);
	}

	void Mesh::init(const std::shared_ptr<Material>& new_material) {
		PROFILER_BEGIN("Mesh", "Mesh Load");

		auto transform = addComponent<Transform>();
		auto material  = addComponent<Material>(new_material);

		M_ASSERT(!_mVertices->empty() && !_mIndices->empty());

		PROFILER_BEGIN("Mesh", "Mesh Texture init");
		for (std::shared_ptr<Core::Texture>& texture : material->getTextures()) {
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

		transform->setHasUpdate(true);
		material->setHasUpdate(true);

		PROFILER_END("Mesh", "Mesh Load");
	}

	void Mesh::onUpdate() {
		for(const auto& component : _mComponents) {
			if (component.second->hasUpdate()) {
				_mHasUpdate = true;
			}

			component.second->onUpdate();
		}

		if (_mHasUpdate) {
			PROFILER_BEGIN("Mesh", "Mesh Update");

			// Reset the update event
			_mHasUpdate = false;

			PROFILER_END("Mesh", "Mesh Update");
		}
	}

	void Mesh::draw(const Core::Shader &shader) const {
		PROFILER_BEGIN("Mesh", "Mesh Draw");

		updateShader(shader);

		Utils::Render::renderer.draw(*_mVA, *_mIB, shader);

		PROFILER_END("Mesh", "Mesh Draw");
	}

	void Mesh::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Mesh", "Mesh Shader Update");

		auto transform = getComponent<Transform>();
		auto material = getComponent<Material>();

		shader.bind();

		material->updateShader(shader);

		shader.setUniformMatrix4fv("uModel", transform->getModel());

		PROFILER_END("Mesh", "Mesh Shader Update");
	}
};
