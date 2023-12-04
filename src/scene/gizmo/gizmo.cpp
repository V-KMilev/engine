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

		auto cubeWorldData = static_cast<ObjectWorldData*>(_mCube->getWorldData().get());

		cubeWorldData->scale = glm::vec3(0.5f, 0.5f, 0.5f);
	}

	const GizmoWorldData& Gizmo::getWorldData() const {
		return _mWolrdData;
	}

	GizmoWorldData& Gizmo::getWorldData() {
		return _mWolrdData;
	}

	const GizmoUseData& Gizmo::getUseData() const {
		return _mUseData;
	}

	GizmoUseData& Gizmo::getUseData() {
		return _mUseData;
	}

	void Gizmo::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		// TODO: Fix depth render index, so it goes above the selected object
		_mCube->draw(renderer, shader);
	}

	void Gizmo::onUpdate(const Mouse* mouse, float deltaTime, const Object& object) {
		auto cubeWorldData   = static_cast<ObjectWorldData*>(_mCube->getWorldData().get());
		auto objectWorldData = static_cast<ObjectWorldData*>(object.getWorldData().get());

		glm::vec3& position = cubeWorldData->position;
		glm::vec3& rotation = cubeWorldData->rotation;

		const glm::vec3& objPosition = objectWorldData->position;
		const glm::vec3& objRotation = objectWorldData->rotation;

		if (position != objPosition) {
			position = objPosition;
			_mCube->getUseData()->hasUpdate = true;
		}
		else if (rotation != objRotation) {
			rotation = objRotation;
			_mCube->getUseData()->hasUpdate = true;
		}

		_mCube->onUpdate(mouse, deltaTime);

		if (_mUseData.useTranslation) {
			_mUseData.useRotation = false;
		}
		else {
			_mUseData.useTranslation = false;
		}
	}
};
