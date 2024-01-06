#include "perspective_camera.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "gl_shader.h"

#include "input_manager.h"

#include "view.h"
#include "object.h"

namespace Engine {
	PerspectiveCamera::PerspectiveCamera(unsigned int width, unsigned int height) : Camera(CameraType::PERSPECTIVE) {
		auto view = getComponent<PerspectiveView>();

		view->on_width  = width;
		view->on_height = height;

		view->projection = glm::perspective(
			glm::radians(view->fov),
			view->on_width / float(view->on_height),
			view->c_near,
			view->c_far
		);

		// Calculate initial camera orientation
		view->on_front = glm::normalize(view->on_target - view->on_position);
		view->on_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), view->on_front));
		view->on_up    = glm::cross(view->on_front, view->on_right);

		// Ensure that the up vector is perpendicular to the direction vector
		if (glm::length(view->on_up) < 0.9f) {
			view->on_right = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), view->on_front));
			view->on_up    = glm::cross(view->on_front, view->on_right);
		}

		view->setHasUpdate(true);
	}

	void PerspectiveCamera::drawUI() const {
		std::string sCamera = "Camera: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sCamera.c_str());

		Entity::drawUI();

		for(const auto& component : _mComponents) {
			component.second->drawUI();
		}
	}

	void PerspectiveCamera::draw(const Core::Shader& shader) const {
		shader.bind();

		_mVisual->draw(shader);
	}

	void PerspectiveCamera::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Camera", "Camera Update Shader");

		shader.bind();

		auto view = getComponent<PerspectiveView>();

		shader.setUniformMatrix4fv("uCamera.view", view->lookAt);
		shader.setUniformMatrix4fv("uCamera.projection", view->projection);

		shader.setUniform1f("uCamera.FOV", view->fov);
		shader.setUniform1f("uCamera.width", (float)view->width);
		shader.setUniform1f("uCamera.height", (float)view->height);

		shader.setUniform1f("uCamera.near", view->c_near);
		shader.setUniform1f("uCamera.far", view->c_far);

		PROFILER_END("Camera", "Camera Update Shader");
	}

	void PerspectiveCamera::updateTarget(const Mouse* mouse, float deltaTime) {
		auto view = getComponent<PerspectiveView>();

		// Calculate the mouse movement angles
		float horizontalDelta = deltaTime * view->mouseSpeed * (view->width / 2.0f - mouse->x);
		float verticalDelta = deltaTime * view->mouseSpeed * (view->height / 2.0f - mouse->y);

		// Update the horizontal & vertical angle
		view->on_horizontalAngle += horizontalDelta;
		view->on_verticalAngle += verticalDelta;

		// Clamp the vertical angle
		view->on_verticalAngle = glm::clamp(view->on_verticalAngle, -view->maxUpAngle, view->maxUpAngle);

		// Calculate the new front direction
		glm::vec3 front(
			cos(view->on_verticalAngle) * sin(view->on_horizontalAngle),
			sin(view->on_verticalAngle),
			cos(view->on_verticalAngle) * cos(view->on_horizontalAngle)
		);

		// front * 2.0f so when you try to move you dont start spining
		view->on_target = view->position + (front * 2.0f);

		view->setHasUpdate(true);
	}

	void PerspectiveCamera::zoom(const Mouse* mouse, float deltaTime) {
		auto view = getComponent<PerspectiveView>();

		if (mouse->hasUpdate) {
			view->on_fov -= deltaTime * view->zoomSpeed * mouse->scrollY;
		}

		if (view->fov > view->maxFOV) {
			view->on_fov = view->maxFOV;
		}
		else if (view->fov < view->minFOV) {
			view->on_fov = view->minFOV;
		}

		view->setHasUpdate(true);
	}
};
