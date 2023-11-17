#include "orientation.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "gl_render.h"
#include "gl_shader.h"

#include "cube.h"

namespace Engine {
	Orientation::Orientation() {
		_mOrientation = std::make_shared<Cube>();
		_mOrientation->getWorldData().position = glm::vec3(0.8f, -0.8f, 0.0f);
		_mOrientation->getWorldData().scale    = glm::vec3(0.07f, 0.07f, 0.07f);
	}


	void Orientation::draw(
		const Core::Renderer& renderer,
		const Core::Shader& shader
	) const {
		shader.bind();

		_mOrientation->draw(renderer, shader);
	}

	void Orientation::onUpdate(const Mouse* mouse, float deltaTime) {
		_mOrientation->onUpdate(mouse, deltaTime);
	}
};