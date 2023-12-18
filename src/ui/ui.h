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
				Scene& scene,
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

			void showUI();

		private:
			void mainManu();

			void leftPanel();
			void rightPanel();

			void lowLeftPanel();
			void LowRightPanel();

			void topPanel();
			void botPanel();

			void addOrRemoveToScene();

			void addModel();
			void remove();

			void saveOrLoadScene();

			void save();
			void load();

		private:
			void setViewPort();

			void setStyle();
		private:
			uiData _mData;

			Scene& _mScene;

			bool _mCallSave = false;
			bool _mCallLoad = false;

			bool _mCallUndo = false;
			bool _mCallRedo = false;

			bool _mCallAddPerspective = false;

			bool _mCallAddCube     = false;
			bool _mCallAddQuad     = false;
			bool _mCallAddTriangle = false;
			bool _mCallAddModel    = false;

			bool _mCallRemove = false;
	};
};
