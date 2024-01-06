#include "object.h"

#include "tracer.h"

#include "gl_render.h"
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

	void Object::onUpdate(const Mouse* mouse, float deltaTime) {
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
				meshTransform->on_position = transform->position;
				meshTransform->on_rotation = transform->rotation;
				meshTransform->on_scale    = transform->scale;

				meshTransform->setHasUpdate(true);

				mesh->onUpdate(mouse, deltaTime);
			}

			// Reset the update event
			_mHasUpdate = false;

			PROFILER_END("Object", "Object Update");
		}
	}

	void Object::drawUI() const {
		std::string sObject = "Object: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sObject.c_str());

		Entity::drawUI();

		for(const auto& component : _mComponents) {
			component.second->drawUI();
		}
	}

	void Object::draw(const Core::Shader &shader) const {
		PROFILER_BEGIN("Object", "Object Draw");

		updateShader(shader);

		auto linesOnly = getComponent<LinesOnly>();

		if (linesOnly->linesOnly) {
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
		shader.setUniform1i("uSelected", _mIsSelected);

		PROFILER_END("Object", "Object Shader Update");
	}
};
