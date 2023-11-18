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
			if (_mUI->getData().isActive) {
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

	void Scene::pickEvent() {
		int x = _mInput->getMouse().x;
		int y = _mHeight - _mInput->getMouse().y - 1;

		Core::PixelInfo pixel = _mPickTexture->readPixel(x, y);

		if (pixel.objectID > 0) {
			for(const std::shared_ptr<Object>& object : _mObjects) {
				object->getUseData().isSelected = (object->getID() == pixel.objectID && !object->getUseData().isSelected) ? true : false;
			}
		}
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

		_mInput->mapKeyandStatetoEvent(GLFW_MOUSE_BUTTON_LEFT, State::PRESS, std::function<void()>( [this] { pickEvent(); } ), "Select ability");
	}
};
