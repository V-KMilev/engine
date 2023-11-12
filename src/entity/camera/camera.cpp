#include "camera.h"

#include "input_handler.h"

#include "error_handle.h"

namespace Engine {
	Camera::Camera(CameraType Type) : _mCameraType(Type), Entity(EntityType::CAMERA) {}

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

		M_ASSERT(mouse);

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

		ud.positionEvent = PositionEvent::NONE;
	}

	void Camera::updateLookAt() {
		auto& wd = _mCameraWorldData;

		wd.lookAt = glm::lookAt(wd.position, wd.target, wd.up);
	}
};
