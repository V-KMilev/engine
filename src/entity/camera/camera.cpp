#include "camera.h"

#include "error_handle.h"
#include "tracer.h"

#include "input_manager.h"

#include "cube.h"

namespace Engine {
	Camera::Camera(CameraType Type) : _mCameraType(Type), Entity(EntityType::CAMERA) {
		// TODO: Fix this when you add more cameras
		_mWorldData = std::make_shared<PerspectiveCameraWorldData>();
		_mUseData   = std::make_shared<PerspectiveCameraUseData>();

		_mVisual = std::make_shared<Cube>();

		auto visualWorldData = static_cast<ObjectWorldData*>(_mVisual->getWorldData().get());

		auto worldData = static_cast<CameraWorldData*>(_mWorldData.get());

		visualWorldData->position = worldData->position;
		visualWorldData->scale    = glm::vec3(0.25f, 0.25f, 0.25f);

		_mVisual->getUseData()->hasUpdate = true;
		_mUseData->hasUpdate = true;
	}

	CameraType Camera::getCameraTpye() const {
		return _mCameraType;
	}

	void Camera::onUpdate(const Mouse* mouse, float deltaTime) {
		if (_mUseData->hasUpdate) {
			PROFILER_BEGIN("Camera", "Camera Update");

			auto useData = static_cast<CameraUseData*>(_mUseData.get());

			switch (useData->updateEvent) {
				case UpdateEvent::POSITION: updatePosition(deltaTime);    break;
				case UpdateEvent::TARGET: updateTarget(mouse, deltaTime); break;
				case UpdateEvent::FOV: zoom(mouse, deltaTime);            break;
				default: break;
			}

			updateProjection();
			updateLookAt();

			_mVisual->onUpdate(mouse, deltaTime);

			// Reset the update event
			useData->updateEvent = UpdateEvent::NONE;
			useData->hasUpdate = false;

			PROFILER_END("Camera", "Camera Update");
		}
	}

	void Camera::updatePosition(float deltaTime) {
		auto worldData = static_cast<CameraWorldData*>(_mWorldData.get());
		auto useData = static_cast<CameraUseData*>(_mUseData.get());

		auto visualWorldData = static_cast<ObjectWorldData*>(_mVisual->getWorldData().get());

		glm::vec3 moveDirection(0.0f);

		switch (useData->positionEvent) {
			case PositionEvent::POSX: moveDirection = worldData->front;  break;
			case PositionEvent::NEGX: moveDirection = -worldData->front; break;
			case PositionEvent::POSY: moveDirection = worldData->up;     break;
			case PositionEvent::NEGY: moveDirection = -worldData->up;    break;
			case PositionEvent::POSZ: moveDirection = worldData->right;  break;
			case PositionEvent::NEGZ: moveDirection = -worldData->right; break;
			default: moveDirection = glm::vec3(0.0f); break;
		}

		worldData->position += deltaTime * useData->moveSpeed * moveDirection;

		// Update the visual representation
		visualWorldData->position = worldData->position;
		_mVisual->getUseData()->hasUpdate = true;

		useData->positionEvent = PositionEvent::NONE;
	}

	void Camera::updateLookAt() {
		auto worldData = static_cast<CameraWorldData*>(_mWorldData.get());

		worldData->lookAt = glm::lookAt(worldData->position, worldData->target, worldData->up);
	}
};
