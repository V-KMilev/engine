#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define MainBarWidth 20
#define ActivityBarWidth 20

namespace Engine {
	struct uiData {
		public:
			uiData(unsigned int width, unsigned int height, bool isActive);

		public:
			unsigned int width = 0;
			unsigned int height = 0;

			bool isActive = true;
	};

	class UI {
		public:
			UI() = delete;
			~UI();

			UI(
				GLFWwindow* window,
				const char* gl_version,
				unsigned int width,
				unsigned int height
			);

			UI(const UI& other) = delete;
			UI operator = (const UI& other) = delete;

			UI(UI && other) = delete;
			UI operator = (UI && other) = delete;

			const uiData& getData() const;
			uiData& getData();

			void newFrame();

			void render();

			void ui();

			void update();

		private:
			void mainManu();
			void leftPanel();
			void rightPanel();
			void topPanel();
			void botPanel();

			void panelPosition();

			void setStyle();
		private:
			uiData _mData;
	};
};
