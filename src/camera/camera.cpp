#include "camera.h"

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

	void Camera::updatePosition(PositionEvent event) {
		if      (event == PositionEvent::POSX) { _mWorldData.position += _mUseData.moveSpeed * _mWorldData.front; }
		else if (event == PositionEvent::NEGX) { _mWorldData.position -= _mUseData.moveSpeed * _mWorldData.front; }
		else if (event == PositionEvent::POSY) { _mWorldData.position += _mUseData.moveSpeed * _mWorldData.up; }
		else if (event == PositionEvent::NEGY) { _mWorldData.position -= _mUseData.moveSpeed * _mWorldData.up; }
		else if (event == PositionEvent::POSZ) { _mWorldData.position += _mUseData.moveSpeed * _mWorldData.right; }
		else if (event == PositionEvent::NEGZ) { _mWorldData.position -= _mUseData.moveSpeed * _mWorldData.right; }
		else {
			return;
		}

		_mWorldData.updateLookAt();
	}

	void Camera::updateTarget(TargetEvent event) {
		if      (event == TargetEvent::POSX) { _mWorldData.target += _mUseData.mouseSpeed * _mWorldData.mouseFront; }
		else if (event == TargetEvent::NEGX) { _mWorldData.target -= _mUseData.mouseSpeed * _mWorldData.mouseFront; }
		else if (event == TargetEvent::POSY) { _mWorldData.target += _mUseData.mouseSpeed * _mWorldData.mouseUp; }
		else if (event == TargetEvent::NEGY) { _mWorldData.target -= _mUseData.mouseSpeed * _mWorldData.mouseUp; }
		else if (event == TargetEvent::POSZ) { _mWorldData.target += _mUseData.mouseSpeed * _mWorldData.mouseRight; }
		else if (event == TargetEvent::NEGZ) { _mWorldData.target -= _mUseData.mouseSpeed * _mWorldData.mouseRight; }
		else {
			return;
		}

		_mWorldData.updateLookAt();
	}
};
