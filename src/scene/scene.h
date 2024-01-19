#pragma once

#include <vector>
#include <memory>

namespace Core {
	class PickTexture;
	class Shader;
}

namespace Engine {
	class SceneManager;
	class EventManager;

	class UI;

	class Orientation;
	class Gizmo;
	class Grid;

	class Entity;
	class Object;
	class Camera;
	class Light;

	enum class UpdateEvent;
	enum class PositionEvent;
}

struct GLFWwindow;

namespace Engine {
	enum class SelectState {
		IDLE       = 0,
		OUTOFFOCUS = 1,
		SELECT     = 2,
		MOVE       = 3
	};

	struct SceneWindow {
		public:
			SceneWindow(unsigned int mainWindowWidth, unsigned int mainWindowHeight);

			void setSceneViewPort(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height);

		public:
			unsigned int width  = 0;
			unsigned int height = 0;

			unsigned int startX = 0;
			unsigned int startY = 0;

		public:
			unsigned int mainWindowWidth  = 0;
			unsigned int mainWindowHeight = 0;
	};

	class Scene {
		public:
			Scene() = delete;
			~Scene() = default;

			Scene(
				GLFWwindow* window,
				const char* gl_version,
				unsigned int width,
				unsigned int height
			);

			Scene(const Scene& other) = delete;
			Scene& operator = (const Scene& other) = delete;

			Scene(Scene && other) = delete;
			Scene& operator = (Scene && other) = delete;

			void render() const;
			void renderUI();

			void onUpdate();

			void removeSelectedEntitys();

			bool isAnythingSelected();

			void setSceneViewPort(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height);

			void addObject(std::shared_ptr<Object> && object);
			void addCamera(std::shared_ptr<Camera> && camera);
			void addLight(std::shared_ptr<Light> && light);
			void addShader(std::shared_ptr<Core::Shader> && shader);

			const std::vector<std::shared_ptr<Entity>>& getEntitys() const;
			std::vector<std::shared_ptr<Entity>>& getEntitys();

			const std::vector<std::shared_ptr<Object>>& getObjects() const;
			std::vector<std::shared_ptr<Object>>& getObjects();

			const std::vector<std::shared_ptr<Camera>>& getCameras() const;
			std::vector<std::shared_ptr<Camera>>& getCameras();

			const std::vector<std::shared_ptr<Core::Shader>>& getShader() const;
			std::vector<std::shared_ptr<Core::Shader>>& getShader();

			const std::shared_ptr<SceneManager>& getSceneManager() const;
			std::shared_ptr<SceneManager>& getSceneManager();

		private:
			void drawOrientation(const std::shared_ptr<Core::Shader>& shader) const;
			void drawGrid(const std::shared_ptr<Core::Shader>& shader) const;
			void drawGizmo(const std::shared_ptr<Core::Shader>& shader) const;

			void drawPick(const std::shared_ptr<Core::Shader>& shader) const;
			void drawSelected(const std::shared_ptr<Core::Shader>& shader) const;

			void drawCameras(const std::shared_ptr<Core::Shader>& shader) const;
			void drawLights(const std::shared_ptr<Core::Shader>& shader) const;
			void drawGeometry(const std::shared_ptr<Core::Shader>& shader) const;

			void pickEntity();
			void moveEntity();

		private:
			void updateCameras(UpdateEvent event, PositionEvent pEvent);

			void keyBinds(GLFWwindow* window);

		private:
			std::shared_ptr<SceneManager> _mSceneManager;
			std::shared_ptr<EventManager> _mEventManager;

			std::shared_ptr<UI> _mUI;

			std::shared_ptr<Core::PickTexture> _mPickTexture;
			std::shared_ptr<Orientation> _mOrientation;
			std::shared_ptr<Grid> _mGrid;
			std::shared_ptr<Gizmo> _mGizmo;

			std::vector<std::shared_ptr<Core::Shader>> _mShaders;

			std::vector<std::shared_ptr<Entity>> _mEntitys;
			std::vector<std::shared_ptr<Object>> _mObjects;
			std::vector<std::shared_ptr<Camera>> _mCameras;
			std::vector<std::shared_ptr<Light>> _mLights;

			SceneWindow _mSceneWindow;

			SelectState _mSelectState = SelectState::IDLE;
	};
}
