#include "object.h"

#include "tracer.h"

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_error_handle.h"

#include "material.h"
#include "mesh.h"

namespace Engine {
	Object::Object(ObjectType type) : _mObjectType(type), Entity(EntityType::OBJECT) {
		_mMaterial = std::make_shared<Material>();

		_mTransform        = std::make_shared<ObjectTransform>();
		_mInteractionState = std::make_shared<ObjectInteractionState>();

		// Initial setup
		_mInteractionState->hasUpdate = true;
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
		if (_mInteractionState->hasUpdate) {
			PROFILER_BEGIN("Object", "Object Update");

			updateModel();

			// TODO: Think of a solution to this update prblem (Idea: to scene update)
			for (std::shared_ptr<Mesh>& mesh : _mMeshes) {
				auto transform = static_cast<ObjectTransform*>(_mTransform.get());

				mesh->updateModel(transform->model);
				mesh->onUpdate(mouse, deltaTime);
			}

			// Reset the update event
			_mInteractionState->hasUpdate = false;

			PROFILER_END("Object", "Object Update");
		}
	}

	void Object::draw(const Core::Shader &shader) const {
		PROFILER_BEGIN("Object", "Object Draw");

		updateShader(shader);

		auto interactionState = static_cast<ObjectInteractionState*>(_mInteractionState.get());

		if (interactionState->linesOnly) {
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
		shader.setUniform1i("uSelected", _mInteractionState->isSelected);

		PROFILER_END("Object", "Object Shader Update");
	}

	void Object::updateModel() {
		PROFILER_BEGIN("Object", "Object Update Model");

		auto transform = static_cast<ObjectTransform*>(_mTransform.get());

		transform->model = glm::mat4(1.0f);

		transform->model = glm::translate(transform->model, transform->position);

		transform->model = glm::rotate(transform->model, glm::radians(transform->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		transform->model = glm::rotate(transform->model, glm::radians(transform->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		transform->model = glm::rotate(transform->model, glm::radians(transform->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		transform->model = glm::scale(transform->model, transform->scale);

		PROFILER_END("Object", "Object Update Model");
	}

	void Object::UIWorld() {
		std::string sObject = "Object: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sObject.c_str());

		if (_mInteractionState->drawUI(_mID.getID())) { _mInteractionState->hasUpdate = true; }
		if (_mTransform->drawUI(_mID.getID())) { _mInteractionState->hasUpdate = true; }
	}
};
