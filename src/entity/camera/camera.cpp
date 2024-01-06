#include "camera.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "error_handle.h"
#include "tracer.h"

#include "input_manager.h"

#include "view.h"
#include "sphere.h"

namespace Engine {
	Camera::Camera(CameraType Type) : _mCameraType(Type), Entity(EntityType::CAMERA) {
		// TODO: Fix this when we have more cameras
		auto view = addComponent<PerspectiveView>();
		addComponent<Activatable>();

		_mVisual = std::make_shared<Sphere>();

		auto visualTransform = _mVisual->getComponent<Transform>();

		visualTransform->setPosition(view->getPosition());
		visualTransform->setScale(glm::vec3(0.25f, 0.25f, 0.25f));

		_mHasUpdate = true;
	}

	CameraType Camera::getCameraType() const {
		return _mCameraType;
	}

	void Camera::setUpdateEvent(UpdateEvent event) {
		_mUpdateEvent = event;
	}

	void Camera::setPositionEvent(PositionEvent event) {
		_mPositionEvent = event;
	}

	void Camera::onUpdate(const Mouse* mouse, float deltaTime) {
		for(const auto& component : _mComponents) {
			if (component.second->hasUpdate()) {
				_mHasUpdate = true;

				// TODO: think of better "hack"
				if(component.second->getType() == ComponentType::VIEW) {
					if (_mUpdateEvent == UpdateEvent::NONE) {
						_mUpdateEvent = UpdateEvent::POSITION;
					}
				}
			}

			component.second->onUpdate();
		}

		if (_mHasUpdate) {
			PROFILER_BEGIN("Camera", "Camera Update");

			switch (_mUpdateEvent) {
				case UpdateEvent::POSITION: updatePosition(deltaTime);      break;
				case UpdateEvent::TARGET:   updateTarget(mouse, deltaTime); break;
				case UpdateEvent::FOV:      zoom(mouse, deltaTime);         break;
				default:                                                    break;
			}

			_mVisual->onUpdate(mouse, deltaTime);

			// Reset the update event
			_mUpdateEvent = UpdateEvent::NONE;
			_mHasUpdate = false;

			PROFILER_END("Camera", "Camera Update");
		}
	}

	void Camera::updatePosition(float deltaTime) {
		auto visualTransform = _mVisual->getComponent<Transform>();

		auto view = getComponent<PerspectiveView>();

		glm::vec3 moveDirection(0.0f);

		switch (_mPositionEvent) {
			case PositionEvent::POSX: moveDirection = view->getFront();  break;
			case PositionEvent::NEGX: moveDirection = -view->getFront(); break;
			case PositionEvent::POSY: moveDirection = view->getUp();     break;
			case PositionEvent::NEGY: moveDirection = -view->getUp();    break;
			case PositionEvent::POSZ: moveDirection = view->getRight();  break;
			case PositionEvent::NEGZ: moveDirection = -view->getRight(); break;
			default: moveDirection = glm::vec3(0.0f); break;
		}

		view->setPosition(view->getPosition() + deltaTime * view->getMoveSpeed() * moveDirection);
		visualTransform->setPosition(view->getPosition());

		// Reset
		_mPositionEvent = PositionEvent::NONE;
	}
};
