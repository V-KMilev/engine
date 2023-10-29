#pragma once

#include <vector>
#include <memory>

namespace Core {
	class Texture;
	class Object;

	class Renderer;
	class Shader;
}

namespace Engine {
	class InputHandle;
}

#include "camera.h"

namespace Engine {
	class Scene {
		public:
			Scene(std::shared_ptr<InputHandle> inputHandler, unsigned int width, unsigned int height);
			~Scene() = default;

			Scene(const Scene& other) = delete;
			Scene& operator = (const Scene& other) = delete;

			Scene(Scene && other) = delete;
			Scene& operator = (Scene && other) = delete;

			void render() const;

			void addObject(std::shared_ptr<Core::Object> && object);
			void addShader(std::shared_ptr<Core::Shader> && shader);
			void addCamera(std::shared_ptr<Camera> && camera);

		private:
			void pickDraw() const;
			void normalDraw() const;

			void keyBinds();

			void update();

			void updateCameras(
				UpdateEvent event,
				PositionEvent pEvent = PositionEvent::NONE,
				TargetEvent tEvent   = TargetEvent::NONE
			);

		private:
			std::shared_ptr<Core::Renderer> _mRenderer;

			std::shared_ptr<InputHandle> _mInput;

			std::vector<std::shared_ptr<Core::Texture>> _mTextures;

			std::vector<std::shared_ptr<Core::Shader>> _mShaders;

			std::vector<std::shared_ptr<Core::Object>> _mObjects;
			std::vector<std::shared_ptr<Camera>> _mCameras;

			unsigned int _mWidth;
			unsigned int _mHeight;
	};
}
