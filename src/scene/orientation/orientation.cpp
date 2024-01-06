#include "orientation.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "gl_render.h"
#include "gl_shader.h"

#include "cube.h"

namespace Engine {
	Orientation::Orientation() {
		_mGeometry = std::make_shared<Cube>();

		auto orientationTransform = _mGeometry->getComponent<Transform>();

		orientationTransform->setPosition(glm::vec3(0.85f, -0.75f, 0.0f));
		orientationTransform->setScale(glm::vec3(0.07f, 0.07f, 0.07f));
	}

	void Orientation::draw(const Core::Shader& shader) const {
		shader.bind();

		_mGeometry->draw(shader);
	}

	void Orientation::onUpdate(const Mouse* mouse, float deltaTime) {
		_mGeometry->onUpdate(mouse, deltaTime);
	}
};
