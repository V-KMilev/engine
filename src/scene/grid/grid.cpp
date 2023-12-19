#include "grid.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "gl_render.h"
#include "gl_shader.h"

#include "quad.h"

namespace Engine {
	Grid::Grid() {
		_mGrid = std::make_shared<Quad>();
	}

	void Grid::draw(const Core::Shader& shader) const {
		shader.bind();

		_mGrid->draw(shader);
	}

	void Grid::onUpdate(const Mouse* mouse, float deltaTime) {
		_mGrid->onUpdate(mouse, deltaTime);
	}
};