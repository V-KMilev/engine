#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include <memory>

namespace Core {
	class Shader;
}

namespace Engine {
	class Object;
}

namespace Engine {
	class Gizmo {
		public:
			Gizmo();
			~Gizmo() = default;

			Gizmo(const Gizmo& other) = delete;
			Gizmo& operator = (const Gizmo& other) = delete;

			Gizmo(Gizmo && other) = delete;
			Gizmo& operator = (Gizmo && other) = delete;

			void onUpdate(const Object& object);

			void draw(const Core::Shader &shader) const;

		private:
			std::shared_ptr<Object> _mGeometry;
	};
};