#pragma once

#include <memory>

namespace Core {
	class Shader;
}

namespace Engine {
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

			void draw(const Core::Shader &shader) const;

			void onUpdate();

		private:
			std::shared_ptr<Object> _mGrid;
	};
};
