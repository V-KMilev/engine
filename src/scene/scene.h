#pragma once

#include <vector>
#include <memory>

namespace Core {
	class Texture;
	class PickTexture;

	class Renderer;
	class Shader;
	class FrameBuffer;
}

namespace Engine {
	class InputHandle;
	class UI;
	class Object;
}

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "camera.h"

namespace Engine {
	class Scene {
		public:
			Scene() = delete;
			~Scene() = default;

			Scene(
				GLFWwindow* window,
				const char* gl_version,
				std::shared_ptr<InputHandle> inputHandler,
				unsigned int width,
				unsigned int height
			);

			Scene(const Scene& other) = delete;
			Scene& operator = (const Scene& other) = delete;

			Scene(Scene && other) = delete;
			Scene& operator = (Scene && other) = delete;

			void render() const;
			void renderUI();

			void update(float deltaTime);

			void addObject(std::shared_ptr<Object> && object);
			void addShader(std::shared_ptr<Core::Shader> && shader);
			void addCamera(std::shared_ptr<Camera> && camera);

		private:
			void drawGrid(const std::shared_ptr<Core::Shader>& shader) const;
			void drawPick(const std::shared_ptr<Core::Shader>& shader) const;
			void drawGeometry(const std::shared_ptr<Core::Shader>& shader) const;

			void pickEvent();

			void updateUI();
			void updateCameras(UpdateEvent event, PositionEvent pEvent = PositionEvent::NONE);

			void keyBinds();

		private:
			std::shared_ptr<InputHandle> _mInput;
			std::shared_ptr<UI> _mUI;

			std::shared_ptr<Core::Renderer> _mRenderer;

			std::shared_ptr<Core::PickTexture> _mPickTexture;
			std::shared_ptr<Object> _mGrid;

			std::vector<std::shared_ptr<Core::Texture>> _mTextures;

			std::vector<std::shared_ptr<Core::Shader>> _mShaders;
			std::vector<std::shared_ptr<Object>> _mObjects;
			std::vector<std::shared_ptr<Camera>> _mCameras;

			float _mDeltaTime;

			unsigned int _mWidth;
			unsigned int _mHeight;
	};
}
