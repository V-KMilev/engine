#pragma once

#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define MainBarWidth 20
#define ActivityBarWidth 20

#include "scene.h"

namespace Engine {
	class Object;
	class Camera;
}

namespace Engine {
	struct uiData {
		public:
			uiData(unsigned int width, unsigned int height, bool isActive);

		public:
			unsigned int width = 0;
			unsigned int height = 0;

		public:
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

			void ui(
				std::vector<std::shared_ptr<Object>>& objects,
				std::vector<std::shared_ptr<Camera>>& cameras
			);

			void update();

		private:
			void mainManu();

			void leftPanel(
				std::vector<std::shared_ptr<Object>>& objects,
				std::vector<std::shared_ptr<Camera>>& cameras
			);
			void rightPanel(
				std::vector<std::shared_ptr<Object>>& objects,
				std::vector<std::shared_ptr<Camera>>& cameras
			);

			void lowLeftPanel(
				std::vector<std::shared_ptr<Object>>& objects,
				std::vector<std::shared_ptr<Camera>>& cameras
			);
			void LowRightPanel(
				std::vector<std::shared_ptr<Object>>& objects,
				std::vector<std::shared_ptr<Camera>>& cameras
			);

			void topPanel(
				std::vector<std::shared_ptr<Object>>& objects,
				std::vector<std::shared_ptr<Camera>>& cameras
			);
			void botPanel(
				std::vector<std::shared_ptr<Object>>& objects,
				std::vector<std::shared_ptr<Camera>>& cameras
			);

			void panelPosition();

			void setStyle();
		private:
			uiData _mData;
	};
};
