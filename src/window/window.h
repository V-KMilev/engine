#pragma once

#include <string>
#include <memory>

struct GLFWwindow;

namespace Engine {
	struct DeltaTime {
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

			void time_tick();

			void set_close(bool flag);

		private:
			GLFWwindow* _mWindow;

			DeltaTime _mDelta;

			std::string _mTitle = "";

			unsigned int _mWidth  = 0;
			unsigned int _mHeight = 0;

			unsigned int _mAA = 0;

			bool _mFullscreen = false;
			bool _mClose      = false;
	};
};
