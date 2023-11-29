#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

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
	struct GizmoWorldData {
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	};

	struct GizmoUseData {
		bool useTranslation = true;
		bool useRotation    = false;
	};

	class Gizmo {
		public:
			Gizmo();
			~Gizmo() = default;

			Gizmo(const Gizmo& other) = delete;
			Gizmo& operator = (const Gizmo& other) = delete;

			Gizmo(Gizmo && other) = delete;
			Gizmo& operator = (Gizmo && other) = delete;

			const GizmoWorldData& getWorldData() const;
			GizmoWorldData& getWorldData();

			const GizmoUseData& getUseData() const;
			GizmoUseData& getUseData();

			void onUpdate(const Mouse* mouse, float deltaTime, const Object& object);

			void draw(const Core::Renderer &renderer, const Core::Shader &shader) const;

		private:
			std::shared_ptr<Object> _mCube;

			GizmoWorldData _mWolrdData;
			GizmoUseData _mUseData;
	};
};