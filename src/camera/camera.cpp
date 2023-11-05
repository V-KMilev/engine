#include "camera.h"

#include "input_handler.h"

namespace Engine {
	void CameraWorldData::updateLookAt() {
		lookAt = glm::lookAt(position, target, up);
	}

	Camera::Camera(CameraType Type) : _mType(Type) {}

	Camera::Camera(const Camera& other) {
		_mID        = other._mID;
		_mWorldData = other._mWorldData;
		_mType      = other._mType;
	}

	Camera& Camera::operator = (const Camera& other) {
		if (this == &other) {
			return *this;
		}
		_mID        = other._mID;
		_mWorldData = other._mWorldData;
		_mType      = other._mType;

		return *this;
	}

	const CameraWorldData& Camera::getWorldData() const {
		return _mWorldData;
	}

	CameraWorldData& Camera::getWorldData() {
		return _mWorldData;
	}

	const UseData& Camera::getUseData() const {
		return _mUseData;
	}
	UseData& Camera::getUseData() {
		return _mUseData;
	}

	void Camera::updatePosition(PositionEvent event, float deltaTime) {
		if      (event == PositionEvent::POSX) { _mWorldData.position += deltaTime * _mUseData.moveSpeed * _mWorldData.front; }
		else if (event == PositionEvent::NEGX) { _mWorldData.position -= deltaTime * _mUseData.moveSpeed * _mWorldData.front; }
		else if (event == PositionEvent::POSY) { _mWorldData.position += deltaTime * _mUseData.moveSpeed * _mWorldData.up;    }
		else if (event == PositionEvent::NEGY) { _mWorldData.position -= deltaTime * _mUseData.moveSpeed * _mWorldData.up;    }
		else if (event == PositionEvent::POSZ) { _mWorldData.position += deltaTime * _mUseData.moveSpeed * _mWorldData.right; }
		else if (event == PositionEvent::NEGZ) { _mWorldData.position -= deltaTime * _mUseData.moveSpeed * _mWorldData.right; }
		else {
			return;
		}

		_mWorldData.updateLookAt();
	}

	void Camera::updateTarget(const Mouse* mouse, float deltaTime, unsigned int width, unsigned int height) {
		// TODO: Fix it

		// Calculate the mouse movement angles
		float horizontalDelta = deltaTime * _mUseData.mouseSpeed * (width / 2.0f - mouse->x);
		float verticalDelta = deltaTime * _mUseData.mouseSpeed * (height / 2.0f - mouse->y);

		// Update the vertical & horizontal angle
		_mUseData.verticalAngle += verticalDelta;
		_mUseData.horizontalAngle += horizontalDelta;

		// Clamp the vertical angle
		_mUseData.verticalAngle = glm::clamp(_mUseData.verticalAngle, -_mUseData.maxUpAngle, _mUseData.maxUpAngle);

		// Calculate the new front direction
		glm::vec3 front(
			cos(_mUseData.verticalAngle) * sin(_mUseData.horizontalAngle),
			sin(_mUseData.verticalAngle),
			cos(_mUseData.verticalAngle) * cos(_mUseData.horizontalAngle)
		);

		// Calculate the new right direction
		glm::vec3 right(
			sin(_mUseData.horizontalAngle - glm::pi<float>() / 2.0f),
			0,
			cos(_mUseData.horizontalAngle - glm::pi<float>() / 2.0f)
		);

		// Calculate the new up direction
		glm::vec3 up = glm::cross(right, front);

		// Update camera properties
		_mWorldData.target = _mWorldData.position + front;

		_mWorldData.updateLookAt();
	}
};
