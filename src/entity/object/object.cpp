#include "object.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "gl_shader.h"

#include "gl_error_handle.h"

#include "material.h"
#include "mesh.h"

namespace Engine {
	Object::Object(ObjectType type) : _mObjectType(type), Entity(EntityType::OBJECT) {
		addComponent<Transform>();
		addComponent<LinesOnly>();

		_mHasUpdate = true;
	}

	ObjectType Object::getObjectType() const {
		return _mObjectType;
	}

	const std::vector<std::shared_ptr<Mesh>>& Object::getMeshes() const {
		return _mMeshes;
	}
	std::vector<std::shared_ptr<Mesh>>& Object::getMeshes() {
		return _mMeshes;
	}

	void Object::onUpdate() {
		for(const auto& component : _mComponents) {
			if (component.second->hasUpdate()) {
				_mHasUpdate = true;
			}

			component.second->onUpdate();
		}

		if (_mHasUpdate) {
			PROFILER_BEGIN("Object", "Object Update");

			for (std::shared_ptr<Mesh>& mesh : _mMeshes) {
				auto transform = getComponent<Transform>();
				auto meshTransform = mesh->getComponent<Transform>();

				// TODO: Add mesh's transform also
				meshTransform->setPosition(transform->getPosition());
				meshTransform->setRotation(transform->getRotation());
				meshTransform->setScale(transform->getScale());

				mesh->onUpdate();
			}

			// Reset the update event
			_mHasUpdate = false;

			PROFILER_END("Object", "Object Update");
		}
	}

	void Object::draw(const Core::Shader &shader) const {
		PROFILER_BEGIN("Object", "Object Draw");

		updateShader(shader);

		auto linesOnly = getComponent<LinesOnly>();

		if (linesOnly->isLinesOnly()) {
			MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		}

		for (const std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->draw(shader);
		}

		MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

		PROFILER_END("Object", "Object Draw");
	}

	void Object::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Object", "Object Shader Update");

		shader.bind();

		shader.setUniform1ui("uObjectID", _mID.getID());

		PROFILER_END("Object", "Object Shader Update");
	}
};
