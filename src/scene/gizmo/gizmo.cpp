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

		transform->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	}

	void Gizmo::draw(const Core::Shader &shader) const {
		// TODO: Fix depth render index, so it goes above the selected object
		_mGeometry->draw(shader);
	}

	void Gizmo::onUpdate(const Mouse* mouse, float deltaTime, const Object& object) {
	}
};
