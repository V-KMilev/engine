#include "scene.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_texture.h"
#include "gl_pick_texture.h"

#include "gl_object.h"
#include "camera.h"

#include "input_handler.h"

namespace Engine {
	Scene::Scene(std::shared_ptr<InputHandle> inputHandle, unsigned int width, unsigned int height) :
		_mWidth(width),
		_mHeight(height),
		_mInput(inputHandle),
		_mTextures({}),
		_mObjects({}),
		_mCameras({}),
		_mShaders({})
	{
		_mRenderer = std::make_shared<Core::Renderer>();

		keyBinds();
	}

	void Scene::render() const {
		_mRenderer->clearColor();
		_mRenderer->clear();

		normalDraw();
	}

	void Scene::addObject(std::shared_ptr<Core::Object> && object) {
		_mObjects.push_back(object);
	}

	void Scene::addShader(std::shared_ptr<Core::Shader> && shader) {
		_mShaders.push_back(shader);
	}

	void Scene::addCamera(std::shared_ptr<Camera> && camera) {
		_mCameras.push_back(camera);
	}

	void Scene::pickDraw() const {
		// Core::PickTexture pTexture;
		// pTexture.enableWriting();

		// _mRenderer->clear();

		// // shader.setUniform1i("gObjectIndex", 1);

		// pTexture.disableWriting();

		// //Render Phase
		// const double& x = _mInput->getMouse().x;
		// const double& y = _mInput->getMouse().y;

		// _mRenderer->clear();

		// // int clicked_object_id = -1;

		// Core::PixelInfo Pixel = pTexture.readPixel(x, _mHeight - y - 1);

		// // if (Pixel.objectID != 0) {
		// // 	clicked_object_id = Pixel.objectID - 1;
		// // }
	}

	void Scene::normalDraw() const {
		for(const std::shared_ptr<Core::Shader>& shader : _mShaders) {

			for(const std::shared_ptr<Core::Object>& object : _mObjects) {
				object->draw(*_mRenderer, *shader);
			}

			for(const std::shared_ptr<Camera>& camera : _mCameras) {
				camera->draw(*shader);
			}
		}
	}

	void Scene::update() {
		// TODO
	}

	void Scene::updateCameras(
		UpdateEvent event,
		PositionEvent pEvent,
		TargetEvent tEvent
	) {
		for(std::shared_ptr<Camera>& camera : _mCameras) {
			if (!camera->getUseData().isActive) {
				camera->update(event, pEvent, tEvent);
			}
		}
	}

	void Scene::keyBinds() {
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_W, State::PRESS, std::function<void()>( [this] {updateCameras(UpdateEvent::POSITION, PositionEvent::POSX);} ), "+x");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_S, State::PRESS, std::function<void()>( [this] {updateCameras(UpdateEvent::POSITION, PositionEvent::NEGX);} ), "-x");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_Q, State::PRESS, std::function<void()>( [this] {updateCameras(UpdateEvent::POSITION, PositionEvent::POSY);} ), "+y");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_E, State::PRESS, std::function<void()>( [this] {updateCameras(UpdateEvent::POSITION, PositionEvent::NEGY);} ), "-y");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_A, State::PRESS, std::function<void()>( [this] {updateCameras(UpdateEvent::POSITION, PositionEvent::POSZ);} ), "+z");
		_mInput->mapKeyandStatetoEvent(GLFW_KEY_D, State::PRESS, std::function<void()>( [this] {updateCameras(UpdateEvent::POSITION, PositionEvent::NEGZ);} ), "-z");

		_mInput->mapKeyandStatetoEvent(GLFW_MOUSE_BUTTON_LEFT, State::PRESS, std::function<void()>( [this] {printf("X: %f, Y: %f\n", _mInput->getMouse().x, _mInput->getMouse().y);} ), "Print mouse position");
	}
};
