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

	unsigned int Camera::getID() const {
		return _mID.getID();
	}

	CameraType Camera::getTpye() const {
		return _mType;
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

	void Camera::update(
		UpdateEvent event,
		PositionEvent pEvent,
		float deltaTime,
		const Mouse* mouse
	) {
		if (event == UpdateEvent::POSITION) {
			_mUseData.hasUpdate = true;
			updatePosition(pEvent, deltaTime);
		}
		else if (event == UpdateEvent::TARGET) {
			_mUseData.hasUpdate = true;
			updateTarget(mouse, deltaTime);
		}
		else if (event == UpdateEvent::FOV) {
			_mUseData.hasUpdate = true;
			zoom(mouse, deltaTime);
		}
		else if (event == UpdateEvent::UI) {
			updateProjection();
			_mWorldData.updateLookAt();
		}
		else {
			return;
		}
		_mUseData.hasUpdate = false;
	}

	void Camera::updatePosition(PositionEvent event, float deltaTime) {
		if(_mUseData.hasUpdate) {
			if      (event == PositionEvent::POSX) { _mWorldData.position += deltaTime * _mUseData.moveSpeed * _mWorldData.front; }
			else if (event == PositionEvent::NEGX) { _mWorldData.position -= deltaTime * _mUseData.moveSpeed * _mWorldData.front; }
			else if (event == PositionEvent::POSY) { _mWorldData.position += deltaTime * _mUseData.moveSpeed * _mWorldData.up;    }
			else if (event == PositionEvent::NEGY) { _mWorldData.position -= deltaTime * _mUseData.moveSpeed * _mWorldData.up;    }
			else if (event == PositionEvent::POSZ) { _mWorldData.position += deltaTime * _mUseData.moveSpeed * _mWorldData.right; }
			else if (event == PositionEvent::NEGZ) { _mWorldData.position -= deltaTime * _mUseData.moveSpeed * _mWorldData.right; }
			else {
				return;
			}

			updateLookAt();
		}
	}

	void Camera::updateLookAt() {
		if(_mUseData.hasUpdate) {
			_mWorldData.updateLookAt();
		}
	}
};
