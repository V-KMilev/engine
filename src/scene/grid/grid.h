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
	class Grid {
		public:
			Grid();
			~Grid() = default;

			Grid(const Grid& other) = delete;
			Grid& operator = (const Grid& other) = delete;

			Grid(Grid && other) = delete;
			Grid& operator = (Grid && other) = delete;

			void draw(const Core::Renderer &renderer, const Core::Shader &shader) const;

			void onUpdate(const Mouse* mouse, float deltaTime);

		private:
			std::shared_ptr<Object> _mGrid;
	};
};
