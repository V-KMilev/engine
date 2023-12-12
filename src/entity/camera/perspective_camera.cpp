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
		// _mWorldData = std::make_shared<PerspectiveCameraWorldData>();
		// _mUseData   = std::make_shared<PerspectiveCameraUseData>();

		auto worldData = static_cast<PerspectiveCameraWorldData*>(_mWorldData.get());

		worldData->width  = width;
		worldData->height = height;

		worldData->projection = glm::perspective(
			glm::radians(worldData->fov),
			worldData->width / float(worldData->height),
			worldData->c_near,
			worldData->c_far
		);

		// Calculate initial camera orientation
		worldData->front = glm::normalize(worldData->target - worldData->position);
		worldData->right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), worldData->front));
		worldData->up    = glm::cross(worldData->front, worldData->right);

		// Ensure that the up vector is perpendicular to the direction vector
		if (glm::length(worldData->up) < 0.9f) {
			worldData->right = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), worldData->front));
			worldData->up    = glm::cross(worldData->front, worldData->right);
		}

		updateLookAt();
	}

	void PerspectiveCamera::draw(const Core::Renderer& renderer, const Core::Shader& shader) const {
		shader.bind();

		_mVisual->draw(renderer, shader);
	}

	void PerspectiveCamera::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Camera", "Camera Update Shader");

		auto worldData = static_cast<PerspectiveCameraWorldData*>(_mWorldData.get());

		shader.bind();

		shader.setUniformMatrix4fv("uCamera.view", worldData->lookAt);
		shader.setUniformMatrix4fv("uCamera.projection", worldData->projection);

		shader.setUniform1f("uCamera.FOV", worldData->fov);
		shader.setUniform1f("uCamera.width", (float)worldData->width);
		shader.setUniform1f("uCamera.height", (float)worldData->height);

		shader.setUniform1f("uCamera.near", worldData->c_near);
		shader.setUniform1f("uCamera.far", worldData->c_far);

		PROFILER_END("Camera", "Camera Update Shader");
	}

	void PerspectiveCamera::updateTarget(const Mouse* mouse, float deltaTime) {
		auto worldData = static_cast<PerspectiveCameraWorldData*>(_mWorldData.get());
		auto useData   = static_cast<PerspectiveCameraUseData*>(_mUseData.get());

		// Calculate the mouse movement angles
		float horizontalDelta = deltaTime * useData->mouseSpeed * (worldData->width / 2.0f - mouse->x);
		float verticalDelta = deltaTime * useData->mouseSpeed * (worldData->height / 2.0f - mouse->y);

		// Update the vertical & horizontal angle
		worldData->verticalAngle += verticalDelta;
		worldData->horizontalAngle += horizontalDelta;

		// Clamp the vertical angle
		worldData->verticalAngle = glm::clamp(worldData->verticalAngle, -worldData->maxUpAngle, worldData->maxUpAngle);

		// Calculate the new front direction
		glm::vec3 front(
			cos(worldData->verticalAngle) * sin(worldData->horizontalAngle),
			sin(worldData->verticalAngle),
			cos(worldData->verticalAngle) * cos(worldData->horizontalAngle)
		);

		// Calculate the new right direction
		glm::vec3 right(
			sin(worldData->horizontalAngle - glm::pi<float>() / 2.0f),
			0,
			cos(worldData->horizontalAngle - glm::pi<float>() / 2.0f)
		);

		// Calculate the new up direction
		glm::vec3 up = glm::cross(right, front);

		// TODO: Think of better way

		// Update camera properties
		// front * 2.0f so when you try to move you dont start spining
		worldData->target = worldData->position + (front * 2.0f);
	}

	void PerspectiveCamera::updateProjection() {
		auto worldData = static_cast<PerspectiveCameraWorldData*>(_mWorldData.get());

		worldData->projection = glm::perspective(
			glm::radians(worldData->fov),
			worldData->width / float(worldData->height),
			worldData->c_near,
			worldData->c_far
		);
	}

	void PerspectiveCamera::zoom(const Mouse* mouse, float deltaTime) {
		auto worldData = static_cast<PerspectiveCameraWorldData*>(_mWorldData.get());
		auto useData   = static_cast<PerspectiveCameraUseData*>(_mUseData.get());

		if (mouse->hasUpdate) {
			worldData->fov -= deltaTime * useData->zoomSpeed * mouse->scrollY;
		}

		if (worldData->fov > worldData->maxFOV) {
			worldData->fov = worldData->maxFOV;
		}
		else if (worldData->fov < worldData->minFOV) {
			worldData->fov = worldData->minFOV;
		}
	}

	void PerspectiveCamera::UIWorld() {
		std::string sCamera = "Camera: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sCamera.c_str());

		if (_mUseData->drawUI(_mID.getID())) { _mUseData->hasUpdate = true; }

		ImGui::SeparatorText("World Data");

		if (_mWorldData->drawUI(_mID.getID())) { _mUseData->hasUpdate = true; }
	}
};
