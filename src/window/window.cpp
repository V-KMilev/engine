#include "window.h"

#ifdef _WIN32
	#include <windows.h>
#endif

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "logger.h"

#include "input_manager.h"
#include "event_manager.h"
#include "scene.h"

#include "perspective_camera.h"
#include "model.h"
#include "cube.h"
#include "quad.h"
#include "sphere.h"

#include "gl_error_handle.h"
#include "gl_shader.h"

namespace Engine {
	Window::Window(
		const std::string& title,
		unsigned int width,
		unsigned int height,
		unsigned int antiAliasing,
		bool fullscreen
	) : _mTitle(title),
		_mWidth(width),
		_mHeight(height),
		_mAA(antiAliasing),
		_mFullscreen(fullscreen),
		_mClose(false) {}

	Window::~Window() {
		deinit();
	}

	bool Window::init() {
		if (!glfwInit()) {
			LOG("Failed to initialize GLFW", LOG_LEVEL::L_ERROR);

			return false;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Anti Aliasing. The second parameter is how many samples per pixels to cast.
		glfwWindowHint(GLFW_SAMPLES, _mAA);

		GLFWmonitor* mode = NULL;

		if (_mFullscreen) {
			mode = glfwGetPrimaryMonitor();
		}

		_mWindow = glfwCreateWindow(
			_mWidth,
			_mHeight,
			_mTitle.c_str(),
			mode,
			nullptr
		);

		if (!_mWindow) {
			LOG("Failed to create GLFW window", LOG_LEVEL::L_ERROR);

			glfwTerminate();
			return false;
		}

		// Make the window's context current
		glfwMakeContextCurrent(_mWindow);
		gladLoadGL(glfwGetProcAddress);
		// FPS cap
		glfwSwapInterval(1);

		input_config();

		return true;
	}

	bool Window::deinit() {
		glfwTerminate();
		return true;
	}

	void Window::gl_config() {
		MY_GL_CHECK(glEnable(GL_PROGRAM_POINT_SIZE));
		MY_GL_CHECK(glEnable(GL_MULTISAMPLE));

		MY_GL_CHECK(glEnable(GL_DEPTH_TEST));
		MY_GL_CHECK(glDepthFunc(GL_LESS));

		// Properly filter across cubemap faces
		MY_GL_CHECK(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));

		// Anti Aliasing
		MY_GL_CHECK(glEnable(GL_MULTISAMPLE));
	}

	void Window::input_config() {
		_mInputManager = std::make_shared<InputManager>();

		glfwSetKeyCallback(_mWindow, _mInputManager->keyCallback);
		glfwSetMouseButtonCallback(_mWindow, _mInputManager->mouseButtonCallback);
		glfwSetScrollCallback(_mWindow, _mInputManager->mouseScrollCallback);

		// TODO: Find better "hack" for this to work
		glfwSetWindowUserPointer(_mWindow, _mInputManager.get());

		// Mapping exit button
		_mInputManager->mapKeyCombinationToEvent(
			{ { {GLFW_KEY_LEFT_CONTROL, State::PRESS}, {GLFW_KEY_C, State::PRESS} } },
			std::function<void()>(
				[this] { set_close(true); }
			),
			"Close main loop"
		);
	}

	float Window::time_tick() {
		_mDelta.currentTime = (float)glfwGetTime();
		_mDelta.deltaTime   = _mDelta.currentTime - _mDelta.lastTime;
		_mDelta.lastTime    = _mDelta.currentTime;

		return _mDelta.deltaTime;
	}

	void Window::set_close(bool flag) {
		LOG("Close event is send", LOG_LEVEL::L_INFO);
		_mClose = flag;
	}

	void Window::main_loop() {
		gl_config();

		std::shared_ptr<Scene> _mScene = std::make_shared<Scene>(
			_mWindow,
			"#version 330",
			_mInputManager,
			_mWidth,
			_mHeight
		);

		_mScene->addCamera(std::make_shared<PerspectiveCamera>(_mWidth, _mHeight));
		_mScene->addObject(std::make_shared<Sphere>());

		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\gizmo"));
		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\infinite_grid"));
		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\orientation"));
		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\pick"));
		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\selected"));
		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\simple_color"));
		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\camera"));
		_mScene->addShader(std::make_shared<Core::Shader>("..\\src\\shaders\\triangle"));

		while(!glfwWindowShouldClose(_mWindow)) {
			_mInputManager->processMouse(_mWindow);

			_mScene->onUpdate(time_tick());

			_mScene->render();
			_mScene->renderUI();

			// Swap front and back buffers
			glfwSwapBuffers(_mWindow);

			// Poll for and process events
			glfwPollEvents();

			if (_mClose) {
				glfwSetWindowShouldClose(_mWindow, _mClose);
			}
		}
	}
};
