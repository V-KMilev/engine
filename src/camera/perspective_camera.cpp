#include "perspective_camera.h"

#include "gl_shader.h"

namespace Engine {
	PerspectiveCamera::PerspectiveCamera(unsigned int width, unsigned int height) : _mWidth(width), _mHeight(height), Camera(CameraType::PERSPECTIVE) {
		_mWorldData.projection = glm::perspective(
			glm::radians(_mFov),
			_mWidth / float(_mHeight),
			_mWorldData.c_near,
			_mWorldData.c_far
		);

		// Calculate initial camera orientation
		_mWorldData.front = glm::normalize(_mWorldData.target - _mWorldData.position);
		_mWorldData.right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), _mWorldData.front));
		_mWorldData.up    = glm::cross(_mWorldData.front, _mWorldData.right);

		// Ensure that the up vector is perpendicular to the direction vector
		if (glm::length(_mWorldData.up) < 0.9f) {
			_mWorldData.right = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), _mWorldData.front));
			_mWorldData.up    = glm::cross(_mWorldData.front, _mWorldData.right);
		}

		_mWorldData.updateLookAt();
	}

	PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& other) : Camera() {
		_mFov = other._mFov;
	}

	PerspectiveCamera& PerspectiveCamera::operator = (const PerspectiveCamera& other) {
		if (this == &other) {
			return *this;
		}

		_mFov = other._mFov;

		return *this;
	}

	float PerspectiveCamera::getFov() const {
		return _mFov;
	}

	float& PerspectiveCamera::getFov() {
		return _mFov;
	}

	void PerspectiveCamera::draw(const Core::Shader& shader) const {
		shader.bind();

		shader.setUniformMatrix4fv("uView", _mWorldData.lookAt);
		shader.setUniformMatrix4fv("uProjection", _mWorldData.projection);

		shader.setUniform1f("uNear", _mWorldData.c_near);
		shader.setUniform1f("uFar", _mWorldData.c_far);
	}

	void PerspectiveCamera::update(
		float deltaTime,
		UpdateEvent event,
		PositionEvent pEvent,
		const Mouse* mouse,
		unsigned int width,
		unsigned int hight
	) {
		if (event == UpdateEvent::POSITION) {
			updatePosition(pEvent, deltaTime);
		}
		else if (event == UpdateEvent::TARGET) {
			updateTarget(mouse, deltaTime, width, hight);
		}
		else if (event == UpdateEvent::C_NEAR) {
			updateProjection();
		}
		else if (event == UpdateEvent::C_FAR) {
			updateProjection();
		}
		else if (event == UpdateEvent::MOVESPEED) {
			// No additional action
		}
		else if (event == UpdateEvent::MOUSESPEED) {
			// No additional action
		}
		else if (event == UpdateEvent::FOV) {
			updateProjection();
		}
		else {
			return;
		}
	}

	void PerspectiveCamera::updateProjection() {
		_mWorldData.projection = glm::perspective(
			glm::radians(_mFov),
			_mWidth / float(_mHeight),
			_mWorldData.c_near,
			_mWorldData.c_far
		);
	}
};
