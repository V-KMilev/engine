#include "perspective_camera.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "gl_shader.h"

#include "input_manager.h"

#include "object.h"

namespace Engine {
	PerspectiveCamera::PerspectiveCamera(unsigned int width, unsigned int height) : Camera(CameraType::PERSPECTIVE) {
		// TODO: Fix this when you add more cameras and fix the Camera class
		// _mTransform        = std::make_shared<PerspectiveCameraTransform>();
		// _mInteractionState = std::make_shared<PerspectiveCameraInteractionState>();

		auto transform = static_cast<PerspectiveCameraTransform*>(_mTransform.get());

		transform->width  = width;
		transform->height = height;

		transform->projection = glm::perspective(
			glm::radians(transform->fov),
			transform->width / float(transform->height),
			transform->c_near,
			transform->c_far
		);

		// Calculate initial camera orientation
		transform->front = glm::normalize(transform->target - transform->position);
		transform->right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), transform->front));
		transform->up    = glm::cross(transform->front, transform->right);

		// Ensure that the up vector is perpendicular to the direction vector
		if (glm::length(transform->up) < 0.9f) {
			transform->right = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), transform->front));
			transform->up    = glm::cross(transform->front, transform->right);
		}

		updateLookAt();
	}

	void PerspectiveCamera::draw(const Core::Renderer& renderer, const Core::Shader& shader) const {
		shader.bind();

		_mVisual->draw(renderer, shader);
	}

	void PerspectiveCamera::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Camera", "Camera Update Shader");

		auto transform = static_cast<PerspectiveCameraTransform*>(_mTransform.get());

		shader.bind();

		shader.setUniformMatrix4fv("uCamera.view", transform->lookAt);
		shader.setUniformMatrix4fv("uCamera.projection", transform->projection);

		shader.setUniform1f("uCamera.FOV", transform->fov);
		shader.setUniform1f("uCamera.width", (float)transform->width);
		shader.setUniform1f("uCamera.height", (float)transform->height);

		shader.setUniform1f("uCamera.near", transform->c_near);
		shader.setUniform1f("uCamera.far", transform->c_far);

		PROFILER_END("Camera", "Camera Update Shader");
	}

	void PerspectiveCamera::updateTarget(const Mouse* mouse, float deltaTime) {
		auto transform        = static_cast<PerspectiveCameraTransform*>(_mTransform.get());
		auto interactionState = static_cast<PerspectiveCameraInteractionState*>(_mInteractionState.get());

		// Calculate the mouse movement angles
		float horizontalDelta = deltaTime * interactionState->mouseSpeed * (transform->width / 2.0f - mouse->x);
		float verticalDelta = deltaTime * interactionState->mouseSpeed * (transform->height / 2.0f - mouse->y);

		// Update the vertical & horizontal angle
		transform->verticalAngle += verticalDelta;
		transform->horizontalAngle += horizontalDelta;

		// Clamp the vertical angle
		transform->verticalAngle = glm::clamp(transform->verticalAngle, -transform->maxUpAngle, transform->maxUpAngle);

		// Calculate the new front direction
		glm::vec3 front(
			cos(transform->verticalAngle) * sin(transform->horizontalAngle),
			sin(transform->verticalAngle),
			cos(transform->verticalAngle) * cos(transform->horizontalAngle)
		);

		// Calculate the new right direction
		glm::vec3 right(
			sin(transform->horizontalAngle - glm::pi<float>() / 2.0f),
			0,
			cos(transform->horizontalAngle - glm::pi<float>() / 2.0f)
		);

		// Calculate the new up direction
		glm::vec3 up = glm::cross(right, front);

		// TODO: Think of better way

		// Update camera properties
		// front * 2.0f so when you try to move you dont start spining
		transform->target = transform->position + (front * 2.0f);
	}

	void PerspectiveCamera::updateProjection() {
		auto transform = static_cast<PerspectiveCameraTransform*>(_mTransform.get());

		transform->projection = glm::perspective(
			glm::radians(transform->fov),
			transform->width / float(transform->height),
			transform->c_near,
			transform->c_far
		);
	}

	void PerspectiveCamera::zoom(const Mouse* mouse, float deltaTime) {
		auto transform        = static_cast<PerspectiveCameraTransform*>(_mTransform.get());
		auto interactionState = static_cast<PerspectiveCameraInteractionState*>(_mInteractionState.get());

		if (mouse->hasUpdate) {
			transform->fov -= deltaTime * interactionState->zoomSpeed * mouse->scrollY;
		}

		if (transform->fov > transform->maxFOV) {
			transform->fov = transform->maxFOV;
		}
		else if (transform->fov < transform->minFOV) {
			transform->fov = transform->minFOV;
		}
	}

	void PerspectiveCamera::UIWorld() {
		std::string sCamera = "Camera: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sCamera.c_str());

		if (_mInteractionState->drawUI(_mID.getID())) { _mInteractionState->hasUpdate = true; }

		ImGui::SeparatorText("Transform");

		if (_mTransform->drawUI(_mID.getID())) { _mInteractionState->hasUpdate = true; }
	}
};
