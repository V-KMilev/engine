#include "gizmo.h"

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_error_handle.h"

#include "input_manager.h"

#include "object.h"
#include "cube.h"

namespace Engine {
	Gizmo::Gizmo() {
		_mCube = std::make_shared<Cube>();

		auto cubeTransform = static_cast<ObjectTransform*>(_mCube->getTransform().get());

		cubeTransform->scale = glm::vec3(0.5f, 0.5f, 0.5f);
	}

	const GizmoTransform& Gizmo::getTransform() const {
		return _mWolrdData;
	}

	GizmoTransform& Gizmo::getTransform() {
		return _mWolrdData;
	}

	const GizmoInteractionState& Gizmo::getInteractionState() const {
		return _mInteractionState;
	}

	GizmoInteractionState& Gizmo::getInteractionState() {
		return _mInteractionState;
	}

	void Gizmo::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		// TODO: Fix depth render index, so it goes above the selected object
		_mCube->draw(renderer, shader);
	}

	void Gizmo::onUpdate(const Mouse* mouse, float deltaTime, const Object& object) {
		auto cubeTransform   = static_cast<ObjectTransform*>(_mCube->getTransform().get());
		auto objectTransform = static_cast<ObjectTransform*>(object.getTransform().get());

		glm::vec3& position = cubeTransform->position;
		glm::vec3& rotation = cubeTransform->rotation;

		const glm::vec3& objPosition = objectTransform->position;
		const glm::vec3& objRotation = objectTransform->rotation;

		if (position != objPosition) {
			position = objPosition;
			_mCube->getInteractionState()->hasUpdate = true;
		}
		else if (rotation != objRotation) {
			rotation = objRotation;
			_mCube->getInteractionState()->hasUpdate = true;
		}

		_mCube->onUpdate(mouse, deltaTime);

		if (_mInteractionState.useTranslation) {
			_mInteractionState.useRotation = false;
		}
		else {
			_mInteractionState.useTranslation = false;
		}
	}
};
