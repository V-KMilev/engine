#include "gizmo.h"

#include "gl_render.h"
#include "gl_shader.h"

namespace Engine {
	Gizmo::Gizmo() {

	}

	const WorldData& Gizmo::getWorldData() const {
		return _mWolrdData;
	}

	WorldData& Gizmo::getWorldData() {
		return _mWolrdData;
	}

	const UseData& Gizmo::getUseData() const {
		return _mUseData;
	}

	UseData& Gizmo::getUseData() {
		return _mUseData;
	}

	void Gizmo::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {

	}

	void Gizmo::onUpdate(const glm::vec3& position, const glm::vec3& rotation) {
		_mWolrdData.position = position;
		_mWolrdData.rotation = rotation;

		if (_mUseData.useTranslation) {
			_mUseData.useRotation = false;
		}
		else {
			_mUseData.useTranslation = false;
		}
	}
};
