#include "scene.h"

#ifdef _WIN32
	#include <windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "gl_error_handle.h"
#include "tracer.h"

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_pick_texture.h"
#include "gl_texture.h"

#include "input_manager.h"
#include "event_manager.h"

#include "ui.h"

#include "orientation.h"
#include "grid.h"
#include "gizmo.h"

#include "camera.h"
#include "perspective_camera.h"

#include "object.h"

namespace Engine {
	Scene::Scene(
		GLFWwindow* window,
		const char* gl_version,
		std::shared_ptr<InputManager> InputManager,
		unsigned int width,
		unsigned int height
	) :
		_mInputManager(InputManager),
		_mWidth(width),
		_mHeight(height),
		_mTextures({}),
		_mObjects({}),
		_mCameras({}),
		_mShaders({})
	{
		// TODO: Make the event system
		_mEventManager = std::make_shared<EventManager>();

		_mUI = std::make_shared<UI>(*this, window, gl_version, _mWidth, _mHeight);

		_mRenderer = std::make_shared<Core::Renderer>();

		_mOrientation = std::make_shared<Orientation>();
		_mGrid = std::make_shared<Grid>();
		_mGizmo = std::make_shared<Gizmo>();

		_mPickTexture = std::make_shared<Core::PickTexture>(_mWidth, _mHeight);

		keyBinds();
	}

	void Scene::render() const {
		_mRenderer->clearColor();
		_mRenderer->clear();

		// TODO: Think of way to update the shaders only if there is a change

		for (const std::shared_ptr<Core::Shader>& shader : _mShaders) {
			// Update the shader with the active camera's data
			for (const std::shared_ptr<Camera>& camera : _mCameras) {
				if (camera->getInteractionState()->isActive) {
					camera->updateShader(*shader);
				}
			}

			if (shader->getName() == "infinite_grid") {
				drawGrid(shader);
			}
			else if (shader->getName() == "orientation") {
				drawOrientation(shader);
			}
			else if (shader->getName() == "gizmo") {
				drawGizmo(shader);
			}
			else if (shader->getName() == "pick") {
				drawPick(shader);
			}
			else if (shader->getName() == "selected") {
				drawSelected(shader);
			}
			else if (shader->getName() == "camera") {
				drawCameras(shader);
			}
			else if (shader->getName() == "triangle") {
				drawGeometry(shader);
			}
		}
	}

	void Scene::renderUI() {
		PROFILER_BEGIN("UI", "UI Draw");

		if (_mUI->getData().isActive) {
			_mUI->newFrame();
			_mUI->ui();
			_mUI->render();
		}

		PROFILER_END("UI", "UI Draw");
	}

	void Scene::onUpdate(float deltaTime) {
		PROFILER_BEGIN("Scene", "Scene Update check");

		_mDeltaTime = deltaTime;

		for (const std::shared_ptr<Camera>& camera : _mCameras) {
			camera->onUpdate(&_mInputManager->getMouse(), _mDeltaTime);
		}

		for (const std::shared_ptr<Object>& object : _mObjects) {
			object->onUpdate(&_mInputManager->getMouse(), _mDeltaTime);

			if (object->getInteractionState()->isSelected) {
				// _mGizmo->onUpdate(&_mInputManager->getMouse(), _mDeltaTime, *object);
			}
		}

		_mOrientation->onUpdate(&_mInputManager->getMouse(), _mDeltaTime);

		// TODO: Think of where this should be
		// Reset the update event
		_mInputManager->getMouse().hasUpdate = false;

		PROFILER_END("Scene", "Scene Update check");
	}

	void Scene::addObject(std::shared_ptr<Object> && object) {
		_mEntitys.push_back(object);
		_mObjects.push_back(object);
	}

	void Scene::addCamera(std::shared_ptr<Camera> && camera) {
		// Set the first camera as main (active) camera
		if (_mCameras.size() == 0) {
			camera->getInteractionState()->isActive = true;
		}

		_mEntitys.push_back(camera);
		_mCameras.push_back(camera);
	}

	void Scene::addShader(std::shared_ptr<Core::Shader> && shader) {
		_mShaders.push_back(shader);
	}


	const std::vector<std::shared_ptr<Entity>>& Scene::getEntitys() const {
		return _mEntitys;
	}
	std::vector<std::shared_ptr<Entity>>& Scene::getEntitys() {
		return _mEntitys;
	}

	const std::vector<std::shared_ptr<Object>>& Scene::getObjects() const {
		return _mObjects;
	}
	std::vector<std::shared_ptr<Object>>& Scene::getObjects() {
		return _mObjects;
	}

	const std::vector<std::shared_ptr<Camera>>& Scene::getCameras() const {
		return _mCameras;
	}
	std::vector<std::shared_ptr<Camera>>& Scene::getCameras() {
		return _mCameras;
	}

	void Scene::drawOrientation(const std::shared_ptr<Core::Shader>& shader) const {
		PROFILER_BEGIN("Orientation", "Orientation Draw");

		_mOrientation->draw(*_mRenderer, *shader);

		PROFILER_END("Orientation", "Orientation Draw");
	}

	void Scene::drawGrid(const std::shared_ptr<Core::Shader>& shader) const {
		PROFILER_BEGIN("Grid", "Draw Grid");

		_mGrid->draw(*_mRenderer, *shader);

		PROFILER_END("Grid", "Draw Grid");
	}

	void Scene::drawGizmo(const std::shared_ptr<Core::Shader>& shader) const {
		PROFILER_BEGIN("Gizmo", "Draw Gizmo");

		for (const std::shared_ptr<Object>& object : _mObjects) {
			if (object->getInteractionState()->isSelected) {
				// _mGizmo->draw(*_mRenderer, *shader);
			}
		}

		PROFILER_END("Gizmo", "Draw Gizmo");
	}

	void Scene::drawPick(const std::shared_ptr<Core::Shader>& shader) const {
		PROFILER_BEGIN("Pick Entity", "Entity Draw Pick");

		_mPickTexture->enableWriting();

		_mRenderer->clear();

		shader->bind();

		drawGeometry(shader);

		_mPickTexture->disableWriting();

		PROFILER_END("Pick Entity", "Entity Draw Pick");
	}

	void Scene::drawSelected(const std::shared_ptr<Core::Shader>& shader) const {
		PROFILER_BEGIN("Pick Entity", "Entity Draw Selected");

		for (const std::shared_ptr<Object>& object : _mObjects) {
			if (object->getInteractionState()->isSelected) {
				MY_GL_CHECK(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
				MY_GL_CHECK(glStencilMask(0x00));
				MY_GL_CHECK(glDisable(GL_DEPTH_TEST));

				object->draw(*_mRenderer, *shader);

				MY_GL_CHECK(glEnable(GL_DEPTH_TEST));
				MY_GL_CHECK(glStencilMask(0xFF));
				MY_GL_CHECK(glStencilFunc(GL_ALWAYS, 0, 0xFF));
			}
		}

		PROFILER_END("Pick Entity", "Entity Draw Selected");
	}

	void Scene::drawCameras(const std::shared_ptr<Core::Shader>& shader) const {
		PROFILER_BEGIN("Cameras", "Cameras Draw");

		for (const std::shared_ptr<Camera>& camera : _mCameras) {
			if (!camera->getInteractionState()->isActive) {
				camera->draw(*_mRenderer, *shader);
			}
		}

		PROFILER_END("Cameras", "Cameras Draw");
	}

	void Scene::drawGeometry(const std::shared_ptr<Core::Shader>& shader) const {
		PROFILER_BEGIN("Geometry", "Geometry Draw");

		for (const std::shared_ptr<Object>& object : _mObjects) {
			object->draw(*_mRenderer, *shader);
		}

		PROFILER_END("Geometry", "Geometry Draw");
	}

	void Scene::pickEntity() {
		PROFILER_BEGIN("Pick Entity", "Entity Pick");

		int x = _mInputManager->getMouse().x;
		int y = _mHeight - _mInputManager->getMouse().y - 1;

		Core::PixelInfo pixel = _mPickTexture->readPixel(x, y);

		if (pixel.objectID > 0) {
			for (const std::shared_ptr<Object>& object : _mObjects) {
				object->getInteractionState()->isSelected = (object->getID() == pixel.objectID && !object->getInteractionState()->isSelected) ? true : false;
			}
		}

		PROFILER_END("Pick Entity", "Entity Pick");
	}

	void Scene::moveEntity() {
		glm::mat4 view       = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		float fov    = 0.0f;
		float width  = 0.0f;
		float height = 0.0f;

		for (const std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getInteractionState()->isActive) {
				auto transform = static_cast<PerspectiveCameraTransform*>(camera->getTransform().get());

				view       = transform->lookAt;
				projection = transform->projection;

				fov    = transform->fov;
				width  = transform->width;
				height = transform->height;
			}
		}

		for (const std::shared_ptr<Object>& object : _mObjects) {
			if (object->getInteractionState()->isSelected) {
				auto transform = static_cast<ObjectTransform*>(object->getTransform().get());
				glm::vec3 position = transform->position;

				float x = (float)_mInputManager->getMouse().x;
				float y = (float)_mInputManager->getMouse().y;

				// Convert mouse coordinates to NDC space
				float xNDC = (2.0f * x) / width - 1.0f;
				// Flip the Y axis
				float yNDC = 1.0f - (2.0f * y) / height;

				// Vector representing the ray in NDC space
				glm::vec4 rayNDC  = glm::vec4(xNDC, yNDC, 1.0f, 1.0f);
				// Convert the ray from NDC space to view space
				glm::vec4 rayView = glm::inverse(projection) * rayNDC;

				// Intersect view vector with object Z plane (in view space)
				glm::vec4 viewPosition = view * glm::vec4(position, 1.0f);
				// Calculate the intersection point of the ray with the object's Z plane in view space
				// Note: I have no idea why we need to get the negative value of viewPosition's z
				// TODO: Find a fix for that, i'm pretty sure its a bug
				glm::vec4 view_space_intersect = glm::vec4(glm::vec3(rayView) * -viewPosition.z, 1.0f);

				transform->position = glm::inverse(view) * view_space_intersect;
				object->getInteractionState()->hasUpdate = true;
			}
		}
	}

	void Scene::updateCameras(UpdateEvent event, PositionEvent pEvent) {
		for (std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getInteractionState()->isActive) {
				camera->getInteractionState()->hasUpdate = true;

				auto interactionState = static_cast<CameraInteractionState*>(camera->getInteractionState().get());

				interactionState->updateEvent = event;
				interactionState->positionEvent = pEvent;
			}
		}
	}

	void Scene::keyBinds() {
		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_W, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSX); } ), "+x");
		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_S, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGX); } ), "-x");
		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_Q, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSY); } ), "+y");
		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_E, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGY); } ), "-y");
		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_A, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSZ); } ), "+z");
		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_D, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGZ); } ), "-z");

		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_U, State::PRESS, std::function<void()>( [this] { _mUI->showUI(); } ), "Activate UI");

		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_LEFT_CONTROL, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::TARGET); } ), "Target move");
		_mInputManager->mapKeyandStatetoEvent(GLFW_KEY_LEFT_ALT, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::FOV); } ), "Zoom");

		_mInputManager->mapKeyandStatetoEvent(GLFW_MOUSE_BUTTON_LEFT, State::PRESS, std::function<void()>( [this] { pickEntity(); } ), "Select ability");
		// TODO: Make the move event on hold
		_mInputManager->mapKeyandStatetoEvent(GLFW_MOUSE_BUTTON_RIGHT, State::PRESS, std::function<void()>( [this] { moveEntity(); } ), "Move ability");
	}
};
