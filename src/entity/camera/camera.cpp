#include "camera.h"

#include "input_manager.h"

#include "cube.h"

#include "error_handle.h"

namespace Engine {
	Camera::Camera(CameraType Type) : _mCameraType(Type), Entity(EntityType::CAMERA) {
		_mCameraUseData.hasUpdate = true;

		_mVisual = std::make_shared<Cube>();

		_mVisual->getWorldData().position = _mCameraWorldData.position;
		_mVisual->getWorldData().scale = glm::vec3(0.25f, 0.25f, 0.25f);
		_mVisual->getUseData().hasUpdate = true;
	}

	CameraType Camera::getCameraTpye() const {
		return _mCameraType;
	}

	const CameraWorldData& Camera::getWorldData() const {
		return _mCameraWorldData;
	}

	CameraWorldData& Camera::getWorldData() {
		return _mCameraWorldData;
	}

	const CameraUseData& Camera::getUseData() const {
		return _mCameraUseData;
	}

	CameraUseData& Camera::getUseData() {
		return _mCameraUseData;
	}

	void Camera::onUpdate(const Mouse* mouse, float deltaTime) {
		auto& ud = _mCameraUseData;

		if (ud.hasUpdate) {
			if (ud.positionEvent != PositionEvent::NONE) {
				updatePosition(deltaTime);
			}
			else if (ud.updateEvent == UpdateEvent::TARGET) {
				updateTarget(mouse, deltaTime);
			}
			else if (ud.updateEvent == UpdateEvent::FOV) {
				zoom(mouse, deltaTime);
			}

			updateProjection();
			updateLookAt();

			ud.updateEvent = UpdateEvent::NONE;

			// Reset the update event
			ud.hasUpdate = false;
		}

		_mVisual->onUpdate(mouse, deltaTime);
	}

	void Camera::updatePosition(float deltaTime) {
		auto& ud = _mCameraUseData;
		auto& wd = _mCameraWorldData;

		if      (ud.positionEvent == PositionEvent::POSX) { wd.position += deltaTime * ud.moveSpeed * wd.front; }
		else if (ud.positionEvent == PositionEvent::NEGX) { wd.position -= deltaTime * ud.moveSpeed * wd.front; }
		else if (ud.positionEvent == PositionEvent::POSY) { wd.position += deltaTime * ud.moveSpeed * wd.up;    }
		else if (ud.positionEvent == PositionEvent::NEGY) { wd.position -= deltaTime * ud.moveSpeed * wd.up;    }
		else if (ud.positionEvent == PositionEvent::POSZ) { wd.position += deltaTime * ud.moveSpeed * wd.right; }
		else if (ud.positionEvent == PositionEvent::NEGZ) { wd.position -= deltaTime * ud.moveSpeed * wd.right; }
		else {
			return;
		}

		_mVisual->getWorldData().position = _mCameraWorldData.position;
		_mVisual->getUseData().hasUpdate = true;

		ud.positionEvent = PositionEvent::NONE;
	}

	void Camera::updateLookAt() {
		auto& wd = _mCameraWorldData;

		wd.lookAt = glm::lookAt(wd.position, wd.target, wd.up);
	}
};
