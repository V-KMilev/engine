#include "gizmo.h"

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_error_handle.h"

#include "input_manager.h"

#include "object.h"
#include "cube.h"

namespace Engine {
	Gizmo::Gizmo() {
		_mGeometry = std::make_shared<Cube>();

		auto transform = _mGeometry->getComponent<Transform>();

		transform->on_scale = glm::vec3(0.5f, 0.5f, 0.5f);

		transform->setHasUpdate(true);
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

	void Gizmo::draw(const Core::Shader &shader) const {
		// TODO: Fix depth render index, so it goes above the selected object
		_mGeometry->draw(shader);
	}

	void Gizmo::onUpdate(const Mouse* mouse, float deltaTime, const Object& object) {
		auto cubeTransform   = _mGeometry->getComponent<Transform>();
		auto objectTransform = object.getComponent<Transform>();

		glm::vec3& position = cubeTransform->position;
		glm::vec3& rotation = cubeTransform->rotation;

		const glm::vec3& objPosition = objectTransform->position;
		const glm::vec3& objRotation = objectTransform->rotation;

		if (position != objPosition) {
			position = objPosition;
			_mGeometry->setHasUpdate(true);
		}
		else if (rotation != objRotation) {
			rotation = objRotation;
			_mGeometry->setHasUpdate(true);
		}

		_mGeometry->onUpdate(mouse, deltaTime);

		if (_mInteractionState.useTranslation) {
			_mInteractionState.useRotation = false;
		}
		else {
			_mInteractionState.useTranslation = false;
		}
	}
};
