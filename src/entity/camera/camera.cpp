#include "camera.h"

#include "error_handle.h"
#include "tracer.h"

#include "input_manager.h"

#include "cube.h"

namespace Engine {
	Camera::Camera(CameraType Type) : _mCameraType(Type), Entity(EntityType::CAMERA) {
		// TODO: Fix this when you add more cameras
		_mTransform        = std::make_shared<PerspectiveCameraTransform>();
		_mInteractionState = std::make_shared<PerspectiveCameraInteractionState>();

		_mVisual = std::make_shared<Cube>();

		auto visualTransform = static_cast<ObjectTransform*>(_mVisual->getTransform().get());

		auto transform = static_cast<CameraTransform*>(_mTransform.get());

		visualTransform->position = transform->position;
		visualTransform->scale    = glm::vec3(0.25f, 0.25f, 0.25f);

		_mVisual->getInteractionState()->hasUpdate = true;
		_mInteractionState->hasUpdate = true;
	}

	CameraType Camera::getCameraTpye() const {
		return _mCameraType;
	}

	void Camera::onUpdate(const Mouse* mouse, float deltaTime) {
		if (_mInteractionState->hasUpdate) {
			PROFILER_BEGIN("Camera", "Camera Update");

			auto interactionState = static_cast<CameraInteractionState*>(_mInteractionState.get());

			switch (interactionState->updateEvent) {
				case UpdateEvent::POSITION: updatePosition(deltaTime);    break;
				case UpdateEvent::TARGET: updateTarget(mouse, deltaTime); break;
				case UpdateEvent::FOV: zoom(mouse, deltaTime);            break;
				default: break;
			}

			updateProjection();
			updateLookAt();

			_mVisual->onUpdate(mouse, deltaTime);

			// Reset the update event
			interactionState->updateEvent = UpdateEvent::NONE;
			interactionState->hasUpdate = false;

			PROFILER_END("Camera", "Camera Update");
		}
	}

	void Camera::updatePosition(float deltaTime) {
		auto transform        = static_cast<CameraTransform*>(_mTransform.get());
		auto interactionState = static_cast<CameraInteractionState*>(_mInteractionState.get());

		auto visualTransform = static_cast<ObjectTransform*>(_mVisual->getTransform().get());

		glm::vec3 moveDirection(0.0f);

		switch (interactionState->positionEvent) {
			case PositionEvent::POSX: moveDirection = transform->front;  break;
			case PositionEvent::NEGX: moveDirection = -transform->front; break;
			case PositionEvent::POSY: moveDirection = transform->up;     break;
			case PositionEvent::NEGY: moveDirection = -transform->up;    break;
			case PositionEvent::POSZ: moveDirection = transform->right;  break;
			case PositionEvent::NEGZ: moveDirection = -transform->right; break;
			default: moveDirection = glm::vec3(0.0f); break;
		}

		transform->position += deltaTime * interactionState->moveSpeed * moveDirection;

		// Update the visual representation
		visualTransform->position = transform->position;
		_mVisual->getInteractionState()->hasUpdate = true;

		interactionState->positionEvent = PositionEvent::NONE;
	}

	void Camera::updateLookAt() {
		auto transform = static_cast<CameraTransform*>(_mTransform.get());

		transform->lookAt = glm::lookAt(transform->position, transform->target, transform->up);
	}
};
