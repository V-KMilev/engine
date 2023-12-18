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
	class SceneManager;
	class InputManager;
	class EventManager;

	class UI;

	class Orientation;
	class Gizmo;
	class Grid;

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
				std::shared_ptr<InputManager> InputManagerr,
				unsigned int width,
				unsigned int height
			);

			Scene(const Scene& other) = delete;
			Scene& operator = (const Scene& other) = delete;

			Scene(Scene && other) = delete;
			Scene& operator = (Scene && other) = delete;

			void render() const;
			void renderUI();

			void onUpdate(float deltaTime);

			void addObject(std::shared_ptr<Object> && object);
			void addCamera(std::shared_ptr<Camera> && camera);
			void addShader(std::shared_ptr<Core::Shader> && shader);

			const std::vector<std::shared_ptr<Entity>>& getEntitys() const;
			std::vector<std::shared_ptr<Entity>>& getEntitys();

			const std::vector<std::shared_ptr<Object>>& getObjects() const;
			std::vector<std::shared_ptr<Object>>& getObjects();

			const std::vector<std::shared_ptr<Camera>>& getCameras() const;
			std::vector<std::shared_ptr<Camera>>& getCameras();

			const std::shared_ptr<SceneManager>& getSceneManager() const;
			std::shared_ptr<SceneManager>& getSceneManager();

			void removeSelectedEntitys();

			bool isAnythingSelected();

		private:
			void drawOrientation(const std::shared_ptr<Core::Shader>& shader) const;
			void drawGrid(const std::shared_ptr<Core::Shader>& shader) const;
			void drawGizmo(const std::shared_ptr<Core::Shader>& shader) const;

			void drawPick(const std::shared_ptr<Core::Shader>& shader) const;
			void drawSelected(const std::shared_ptr<Core::Shader>& shader) const;

			void drawCameras(const std::shared_ptr<Core::Shader>& shader) const;
			void drawGeometry(const std::shared_ptr<Core::Shader>& shader) const;

			void pickEntity();
			void moveEntity();

		private:
			void updateCameras(UpdateEvent event, PositionEvent pEvent = PositionEvent::NONE);

			void keyBinds();

		private:
			std::shared_ptr<SceneManager> _mSceneManager;
			std::shared_ptr<InputManager> _mInputManager;
			std::shared_ptr<EventManager> _mEventManager;

			std::shared_ptr<Core::Renderer> _mRenderer;
			std::shared_ptr<Core::PickTexture> _mPickTexture;
			std::shared_ptr<UI> _mUI;

			std::shared_ptr<Orientation> _mOrientation;
			std::shared_ptr<Grid> _mGrid;
			std::shared_ptr<Gizmo> _mGizmo;

			std::vector<std::shared_ptr<Core::Texture>> _mTextures;

			std::vector<std::shared_ptr<Core::Shader>> _mShaders;

			std::vector<std::shared_ptr<Entity>> _mEntitys;

			std::vector<std::shared_ptr<Object>> _mObjects;
			std::vector<std::shared_ptr<Camera>> _mCameras;

			float _mDeltaTime;

			unsigned int _mWidth;
			unsigned int _mHeight;
	};
}
