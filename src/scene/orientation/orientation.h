#pragma once

#include <memory>

namespace Core {
	class Renderer;
	class Shader;
}

namespace Engine {
	struct Mouse;
	class Object;
}

namespace Engine {
	class Orientation {
		public:
			Orientation();
			~Orientation() = default;

			Orientation(const Orientation& other) = delete;
			Orientation& operator = (const Orientation& other) = delete;

			Orientation(Orientation && other) = delete;
			Orientation& operator = (Orientation && other) = delete;

			void draw(const Core::Renderer &renderer, const Core::Shader &shader) const;

			void onUpdate(const Mouse* mouse, float deltaTime);

		private:
			std::shared_ptr<Object> _mOrientation;
	};
};
