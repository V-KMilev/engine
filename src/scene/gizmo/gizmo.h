#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

namespace Core {
	class Renderer;
	class Shader;
}

namespace Engine {
	struct Mouse;
	class Object;
}

namespace Engine {
	struct WorldData {
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	};

	struct UseData {
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

			const WorldData& getWorldData() const;
			WorldData& getWorldData();

			const UseData& getUseData() const;
			UseData& getUseData();

			void draw(const Core::Renderer &renderer, const Core::Shader &shader) const;

			void onUpdate(const glm::vec3& position, const glm::vec3& rotation);

		private:
			WorldData _mWolrdData;
			UseData _mUseData;
	};
};