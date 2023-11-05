#pragma once

#include <vector>
#include <memory>

namespace Core {
	class Texture;
	class PickTexture;
	class Object;

	class Renderer;
	class Shader;
	class FrameBuffer;
}

namespace Engine {
	class InputHandle;
	class UI;
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

			void addObject(std::shared_ptr<Core::Object> && object);
			void addShader(std::shared_ptr<Core::Shader> && shader);
			void addCamera(std::shared_ptr<Camera> && camera);

			void updateTime(float deltaTime);

		private:
			void drawGrid(const std::shared_ptr<Core::Shader>& shader) const;
			void pickDraw(const std::shared_ptr<Core::Shader>& shader) const;
			void draw(const std::shared_ptr<Core::Shader>& shader) const;
			void drawUI() const;

			void update();

			void pickEvent();

			void updateUI();

			void updateCameras(
				UpdateEvent event,
				PositionEvent pEvent = PositionEvent::NONE
			);

			void keyBinds(GLFWwindow* window);

		private:
			std::shared_ptr<InputHandle> _mInput;
			std::shared_ptr<UI> _mUI;

			std::shared_ptr<Core::Renderer> _mRenderer;

			std::shared_ptr<Core::PickTexture> _mPickTexture;
			std::shared_ptr<Core::FrameBuffer> FB;

			std::vector<std::shared_ptr<Core::Texture>> _mTextures;

			std::vector<std::shared_ptr<Core::Shader>> _mShaders;
			std::vector<std::shared_ptr<Core::Object>> _mObjects;
			std::vector<std::shared_ptr<Camera>> _mCameras;

			std::shared_ptr<Core::Object> _mGrid;

			float _mDeltaTime;

			unsigned int _mWidth;
			unsigned int _mHeight;
	};
}
