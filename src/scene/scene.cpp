#include "scene.h"

#ifdef _WIN32
	#include <windows.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "gl_error_handle.h"

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_pick_texture.h"
#include "gl_texture.h"

#include "orientation.h"
#include "grid.h"

#include "object.h"

#include "input_handler.h"
#include "ui.h"

#include "camera.h"
#include "perspective_camera.h"

namespace Engine {
	Scene::Scene(
		GLFWwindow* window,
		const char* gl_version,
		std::shared_ptr<InputHandle> inputHandle,
		unsigned int width,
		unsigned int height
	) :
		_mInput(inputHandle),
		_mWidth(width),
		_mHeight(height),
		_mTextures({}),
		_mObjects({}),
		_mCameras({}),
		_mShaders({})
	{
		_mUI = std::make_shared<UI>(window, gl_version, _mWidth, _mHeight);

		_mRenderer = std::make_shared<Core::Renderer>();

		_mOrientation = std::make_shared<Orientation>();
		_mGrid = std::make_shared<Grid>();

		_mPickTexture = std::make_shared<Core::PickTexture>(_mWidth, _mHeight);

		keyBinds();
	}

	void Scene::render() const {
		_mRenderer->clearColor();
		_mRenderer->clear();

		for(const std::shared_ptr<Core::Shader>& shader : _mShaders) {
			if (shader->getName() == "infinite_grid") {
				drawGrid(shader);
			}
			else if (shader->getName() == "orientation") {
				drawOrientation(shader);
			}
			else if (shader->getName() == "pick") {
				drawPick(shader);
			}
			else if(shader->getName() == "triangle") {
				moveEntity(shader);
				drawGeometry(shader);
			}
		}
	}

	void Scene::renderUI() {
		if (_mUI->getData().isActive) {
			_mUI->newFrame();
			_mUI->ui(_mObjects, _mCameras);
			_mUI->render();
		}
	}

	void Scene::onUpdate(float deltaTime) {
		_mDeltaTime = deltaTime;

		for(const std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getUseData().isActive) {
				camera->onUpdate(&_mInput->getMouse(), _mDeltaTime);
			}
		}

		for(const std::shared_ptr<Object>& object : _mObjects) {
			object->onUpdate(&_mInput->getMouse(), _mDeltaTime);
		}

		_mOrientation->onUpdate(&_mInput->getMouse(), _mDeltaTime);

		// TODO: Think of where this should be
		// Reset the update event
		_mInput->getMouse().hasUpdate = false;
	}

	void Scene::addObject(std::shared_ptr<Object> && object) {
		_mObjects.push_back(object);
	}

	void Scene::addShader(std::shared_ptr<Core::Shader> && shader) {
		_mShaders.push_back(shader);
	}

	void Scene::addCamera(std::shared_ptr<Camera> && camera) {
		// Set the first camera as main (active) camera
		if(_mCameras.size() == 0) {
			camera->getUseData().isActive = true;
		}

		_mCameras.push_back(camera);
	}

	void Scene::drawView(const std::shared_ptr<Core::Shader>& shader) const {
		shader->bind();

		for(const std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getUseData().isActive) {
				camera->draw(*_mRenderer, *shader);
			}
		}
	}

	void Scene::drawGrid(const std::shared_ptr<Core::Shader>& shader) const {
		drawView(shader);

		_mGrid->draw(*_mRenderer, *shader);
	}

	void Scene::drawOrientation(const std::shared_ptr<Core::Shader>& shader) const {
		drawView(shader);

		_mOrientation->draw(*_mRenderer, *shader);
	}

	void Scene::drawPick(const std::shared_ptr<Core::Shader>& shader) const {
		_mPickTexture->enableWriting();

		_mRenderer->clear();

		shader->bind();

		drawGeometry(shader);

		_mPickTexture->disableWriting();
	}

	void Scene::drawGeometry(const std::shared_ptr<Core::Shader>& shader) const {
		drawView(shader);

		for(const std::shared_ptr<Object>& object : _mObjects) {
			object->draw(*_mRenderer, *shader);
		}
	}

	void Scene::pickEntity() {
		int x = _mInput->getMouse().x;
		int y = _mHeight - _mInput->getMouse().y - 1;

		Core::PixelInfo pixel = _mPickTexture->readPixel(x, y);

		if (pixel.objectID > 0) {
			for(const std::shared_ptr<Object>& object : _mObjects) {
				object->getUseData().isSelected = (object->getID() == pixel.objectID && !object->getUseData().isSelected) ? true : false;
			}
		}
	}

	void Scene::moveEntity(const std::shared_ptr<Core::Shader>& shader) const {
		// Function to move an entity in world space based on mouse coordinates

		glm::mat4 view       = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		float fov    = 0.0f;
		float width  = 0.0f;
		float height = 0.0f;

		for(const std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getUseData().isActive) {
				view       = camera->getWorldData().lookAt;
				projection = camera->getWorldData().projection;

				PerspectiveCamera* cam = static_cast<PerspectiveCamera*>(camera.get());

				fov    = cam->getFov();
				width  = cam->getWidth();
				height = cam->getHeight();
			}
		}

		for(const std::shared_ptr<Object>& object : _mObjects) {
			if (object->getUseData().isSelected && _mMoveWithMouse) {
				// Get the position of the object in world space
				glm::vec3 position = object->getWorldData().position;

				// Get mouse coordinates
				float x = (float)_mInput->getMouse().x;
				float y = (float)_mInput->getMouse().y;

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
				glm::vec4 view_space_intersect = glm::vec4(glm::vec3(rayView) * -viewPosition.z, 1.0f);

				object->getWorldData().position = glm::inverse(view) * view_space_intersect;
				object->getUseData().hasUpdate = true;
			}
		}
	}

	void Scene::moveEvent() {
		_mMoveWithMouse = _mMoveWithMouse ? false : true;
	}

	void Scene::updateCameras(UpdateEvent event, PositionEvent pEvent) {
		for(std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getUseData().isActive) {
				camera->getUseData().hasUpdate = true;
				camera->getUseData().updateEvent = event;
				camera->getUseData().positionEvent = pEvent;
			}
		}
	}

	void Scene::keyBinds() {
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_W, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSX); } ), "+x");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_S, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGX); } ), "-x");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_Q, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSY); } ), "+y");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_E, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGY); } ), "-y");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_A, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSZ); } ), "+z");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_D, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGZ); } ), "-z");

		_mInput->mapKeyandStatetoEvent(GLFW_KEY_U, State::PRESS, std::function<void()>( [this] { _mUI->showUI(); } ), "Activate UI");

		_mInput->mapKeyandStatetoEvent(GLFW_KEY_LEFT_CONTROL, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::TARGET); } ), "Target move");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_LEFT_ALT, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::FOV); } ), "Zoom");

		_mInput->mapKeyandStatetoEvent(GLFW_MOUSE_BUTTON_LEFT, State::PRESS, std::function<void()>( [this] { pickEntity(); } ), "Select ability");
		_mInput->mapKeyandStatetoEvent(GLFW_MOUSE_BUTTON_RIGHT, State::PRESS, std::function<void()>( [this] { moveEvent(); } ), "Move flag");
	}
};
