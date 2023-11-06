#include "perspective_camera.h"

#include "gl_shader.h"

#include "input_handler.h"

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

		_mUseData.hasUpdate = true;
		updateLookAt();
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

	unsigned int PerspectiveCamera::getWidth() const {
		return _mWidth;
	}
	unsigned int& PerspectiveCamera::getWidth() {
		return _mWidth;
	}

	unsigned int PerspectiveCamera::getHeight() const {
		return _mHeight;
	}
	unsigned int& PerspectiveCamera::getHeight() {
		return _mHeight;
	}

	void PerspectiveCamera::draw(const Core::Shader& shader) const {
		shader.bind();

		shader.setUniformMatrix4fv("uView", _mWorldData.lookAt);
		shader.setUniformMatrix4fv("uProjection", _mWorldData.projection);

		shader.setUniform1f("uNear", _mWorldData.c_near);
		shader.setUniform1f("uFar", _mWorldData.c_far);
	}

	void PerspectiveCamera::updateTarget(const Mouse* mouse, float deltaTime) {
		if(_mUseData.hasUpdate) {
			// Calculate the mouse movement angles
			float horizontalDelta = deltaTime * _mUseData.mouseSpeed * (_mWidth / 2.0f - mouse->x);
			float verticalDelta = deltaTime * _mUseData.mouseSpeed * (_mHeight / 2.0f - mouse->y);

			// Update the vertical & horizontal angle
			_mWorldData.verticalAngle += verticalDelta;
			_mWorldData.horizontalAngle += horizontalDelta;

			// Clamp the vertical angle
			_mWorldData.verticalAngle = glm::clamp(_mWorldData.verticalAngle, -_mWorldData.maxUpAngle, _mWorldData.maxUpAngle);

			// Calculate the new front direction
			glm::vec3 front(
				cos(_mWorldData.verticalAngle) * sin(_mWorldData.horizontalAngle),
				sin(_mWorldData.verticalAngle),
				cos(_mWorldData.verticalAngle) * cos(_mWorldData.horizontalAngle)
			);

			// Calculate the new right direction
			glm::vec3 right(
				sin(_mWorldData.horizontalAngle - glm::pi<float>() / 2.0f),
				0,
				cos(_mWorldData.horizontalAngle - glm::pi<float>() / 2.0f)
			);

			// Calculate the new up direction
			glm::vec3 up = glm::cross(right, front);

			// Update camera properties
			_mWorldData.target = _mWorldData.position + front;

			updateLookAt();
		}
	}

	void PerspectiveCamera::updateProjection() {
		if( _mUseData.hasUpdate) {
			_mWorldData.projection = glm::perspective(
				glm::radians(_mFov),
				_mWidth / float(_mHeight),
				_mWorldData.c_near,
				_mWorldData.c_far
			);
		}
	}

	void PerspectiveCamera::zoom(const Mouse* mouse, float deltaTime) {
		_mFov -= mouse->scrollY; //* deltaTime;

		if (_mFov < 1.0f) {
			_mFov = 10.0f;
		}
		else if(_mFov > 120.0f) {
			_mFov = 120.0f;
		}

		updateProjection();
	}
};
