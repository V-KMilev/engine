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

#include "gl_object.h"
#include "gl_quad.h"

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
		_mWidth(width),
		_mHeight(height),
		_mInput(inputHandle),
		_mTextures({}),
		_mObjects({}),
		_mCameras({}),
		_mShaders({})
	{
		_mUI = std::make_shared<UI>(window, gl_version, _mWidth, _mHeight);

		_mRenderer = std::make_shared<Core::Renderer>();

		_mPickTexture = std::make_shared<Core::PickTexture>(_mWidth, _mHeight);
		_mGrid = std::make_shared<Core::Quad>();

		keyBinds(window);
	}

	void Scene::render() const {
		_mRenderer->clearColor();
		_mRenderer->clear();

		for(const std::shared_ptr<Core::Shader>& shader : _mShaders) {
			if (shader->getName() == "infinite_grid") {
				drawGrid(shader);
			}
			else if (shader->getName() == "pick") {
				pickDraw(shader);
			}
			else if(shader->getName() == "triangle") {
				draw(shader);
			}
		}

		drawUI();
	}

	void Scene::addObject(std::shared_ptr<Core::Object> && object) {
		_mObjects.push_back(object);
	}

	void Scene::addShader(std::shared_ptr<Core::Shader> && shader) {
		_mShaders.push_back(shader);
	}

	void Scene::addCamera(std::shared_ptr<Camera> && camera) {
		if(_mCameras.size() == 0) {
			camera->getUseData().isActive = true;
		}

		_mCameras.push_back(camera);
	}

	void Scene::updateTime(float deltaTime) {
		_mDeltaTime = deltaTime;
	}

	void Scene::drawGrid(const std::shared_ptr<Core::Shader>& shader) const {
		shader->bind();

		for(const std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getUseData().isActive) {
				camera->draw(*shader);
			}
		}

		_mGrid->draw(*_mRenderer, *shader);
	}

	void Scene::pickDraw(const std::shared_ptr<Core::Shader>& shader) const {
		_mPickTexture->enableWriting();

		_mRenderer->clear();

		shader->bind();

		draw(shader);

		_mPickTexture->disableWriting();
	}

	void Scene::draw(const std::shared_ptr<Core::Shader>& shader) const {
		for(const std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getUseData().isActive) {
				camera->draw(*shader);
			}
		}

		for(const std::shared_ptr<Core::Object>& object : _mObjects) {
			shader->setUniform1ui("uObjectID", object->getID());
			object->draw(*_mRenderer, *shader);
		}
	}

	void Scene::drawUI() const {
		if (_mUI->getData().isActive) {
			_mUI->newFrame();
			_mUI->ui();
			_mUI->render();
		}
	}

	void Scene::update() {
		// TODO
	}

	void Scene::pickEvent() {
		int x = _mInput->getMouse().x;
		int y = _mHeight - _mInput->getMouse().y - 1;

		Core::PixelInfo pixel = _mPickTexture->readPixel(x, y);

		if (pixel.objectID > 0) {
			// TODO: Add pick logic
		}
	}

	void Scene::updateUI() {
		if (!_mUI->getData().isActive) {
			_mUI->getData().isActive = true;
			MY_GL_CHECK(glViewport(
				_mWidth  * 1 / 4,
				_mHeight * 2 / 8,
				_mWidth  * 2 / 4,
				_mHeight * 2 / 4
			));
		}
		else {
			_mUI->getData().isActive = false;

			MY_GL_CHECK(glViewport(0, 0, _mWidth, _mHeight));
		}
	}

	void Scene::updateCameras(
		UpdateEvent event,
		PositionEvent pEvent
	) {
		for(std::shared_ptr<Camera>& camera : _mCameras) {
			if (camera->getUseData().isActive) {
				camera->update(_mDeltaTime, event, pEvent, &_mInput->getMouse(), _mWidth, _mHeight);
			}
		}
	}

	void Scene::keyBinds(GLFWwindow* window) {
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_W, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSX); } ), "+x");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_S, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGX); } ), "-x");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_Q, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSY); } ), "+y");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_E, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGY); } ), "-y");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_A, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::POSZ); } ), "+z");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_D, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::POSITION, PositionEvent::NEGZ); } ), "-z");

		_mInput->mapKeyandStatetoEvent(GLFW_KEY_U, State::PRESS, std::function<void()>( [this] { updateUI(); } ), "Activate UI");

		_mInput->mapKeyandStatetoEvent(GLFW_KEY_LEFT_CONTROL, State::PRESS, std::function<void()>( [this] { updateCameras(UpdateEvent::TARGET); } ), "View move");
		_mInput->mapKeyandStatetoEvent(GLFW_MOUSE_BUTTON_LEFT, State::PRESS, std::function<void()>( [this] { pickEvent(); } ), "Select ability");
	}
};
