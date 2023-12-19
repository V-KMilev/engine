#pragma once

#include <memory>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "entity.h"

namespace Engine {
	class Object;

	struct Mouse;
};

namespace Engine {
	enum class CameraType {
		NONE        = 0,
		PERSPECTIVE = 1
	};

	class Camera : public Entity {
		public:
			Camera(CameraType Type = CameraType::NONE);

			CameraType getCameraType() const;

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			virtual void draw(const Core::Renderer &renderer, const Core::Shader &shader) const override = 0;

			virtual void updateShader(const Core::Shader &shader) const override = 0;

			virtual void UIWorld() override = 0;

		protected:
			void updatePosition(float deltaTime);
			void updateLookAt();

			virtual void updateTarget(const Mouse* mouse, float deltaTime) = 0;

			virtual void updateProjection() = 0;

			virtual void zoom(const Mouse* mouse, float deltaTime) = 0;

		protected:
			CameraType _mCameraType;

			std::shared_ptr<Object> _mVisual;
	};
}
