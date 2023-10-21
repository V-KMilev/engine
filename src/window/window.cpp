#include "window.h"

#ifdef _WIN32
	#include <windows.h>
#endif

#include "logger.h"

namespace Engine {
	Window::Window(
		const std::string& title,
		uint32_t width,
		uint32_t height,
		uint32_t antiAliasing,
		bool fullscreen
	) : _title(title),
		_width(width),
		_height(height),
		_AA(antiAliasing),
		_fullscreen(fullscreen),
		_close(false) {}

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
		glfwWindowHint(GLFW_SAMPLES, _AA);

		GLFWmonitor* mode = NULL;

		if (_fullscreen) {
			mode = glfwGetPrimaryMonitor();
		}

		_window = glfwCreateWindow(
			_width,
			_height,
			_title.c_str(),
			mode,
			nullptr
		);

		if (!_window) {
			LOG("Failed to create GLFW window", LOG_LEVEL::L_ERROR);

			glfwTerminate();
			return false;
		}

		// Make the window's context current
		glfwMakeContextCurrent(_window);
		gladLoadGL(glfwGetProcAddress);
		// FPS cap
		glfwSwapInterval(0);

		// TODO: Find better hack for this to work
		glfwSetKeyCallback(_window, _input.keyCallback);
		glfwSetWindowUserPointer(_window, &_input);

		// Mapping exit button
		_input.mapKeyandStatetoEvent(
			GLFW_KEY_C,
			STATE::PRESS,
			std::function<void()>(
				[this] { set_close(true); }
			)
		);

		return true;
	}

	bool Window::deinit() {
		glfwTerminate();
		return true;
	}

	void Window::set_close(bool flag) {
		LOG("Close event is send", LOG_LEVEL::L_INFO);
		_close = flag;
	}

	void Window::main_loop() {
		while(!glfwWindowShouldClose(_window)) {
			/* Swap front and back buffers */
			glfwSwapBuffers(_window);

			/* Poll for and process events */
			glfwPollEvents();

			if(_close) {
				glfwSetWindowShouldClose(_window, _close);
			}
		}
	}
};
