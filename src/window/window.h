#pragma once

#include <string>
#include <memory>

namespace Engine {
	class InputHandle;
}

// To avoid including the full GLFW implementation, we forward declaration GLFWwindow
class GLFWwindow;

namespace Engine {
	struct deltaTime {
		float lastTime    = 0.0f;
		float currentTime = 0.0f;
		float deltaTime   = 0.0f;
	};

	class Window {
		public:
			Window(
				const std::string& title,
				unsigned int width,
				unsigned int height,
				unsigned int antiAliasing,
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
			void gl_config();

			void input_config();

			void set_close(bool flag);

			float time_tick();

		private:
			deltaTime _mDelta;

			GLFWwindow* _mWindow;

			std::shared_ptr<InputHandle> _mInput;

			std::string _mTitle = "";

			unsigned int _mWidth  = 0;
			unsigned int _mHeight = 0;

			unsigned int _mAA = 0;

			bool _mFullscreen = false;
			bool _mClose      = false;
	};
};
