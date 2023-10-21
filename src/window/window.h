#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

#include "input_handler.h"

namespace Engine {
	class Window {
		public:
			Window(
				const std::string& title,
				uint32_t width,
				uint32_t height,
				uint32_t antiAliasing,
				bool fullscreen
			);
			~Window();

			Window(const Window& other) = delete;
			Window& operator = (const Window& other) = delete;

			Window(Window && other) = delete;
			Window& operator = (Window && other) = delete;

			bool init();
			bool deinit();

			void main_loop();

		private:
			void set_close(bool flag);

		private:
			GLFWwindow* _window;

			std::string _title;
			uint32_t _width;
			uint32_t _height;

			uint32_t _AA;

			bool _fullscreen;
			bool _close;

			InputHandle _input;
	};
};
