#pragma once

#include <memory>

namespace Core {
	class Shader;
}

namespace Engine {
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

			void draw(const Core::Shader &shader) const;

			void onUpdate();

		private:
			std::shared_ptr<Object> _mGeometry;
	};
};
