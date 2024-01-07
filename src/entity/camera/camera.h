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

	enum class UpdateEvent {
		NONE       = 0,
		POSITION   = 1,
		TARGET     = 2,
		FOV        = 3
	};

	enum class PositionEvent {
		NONE = 0,
		POSX = 1,
		NEGX = 2,
		POSY = 3,
		NEGY = 4,
		POSZ = 5,
		NEGZ = 6
	};

	class Camera : public Entity {
		public:
			Camera() = delete;
			~Camera() = default;

			Camera(CameraType Type);

			Camera(const Camera& other) = delete;
			Camera& operator = (const Camera& other) = delete;

			Camera(Camera && other) = delete;
			Camera& operator = (Camera && other) = delete;

			CameraType getCameraType() const;

			void setUpdateEvent(UpdateEvent event);
			void setPositionEvent(PositionEvent event);

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			virtual void drawUI() const override = 0;
			virtual void draw(const Core::Shader &shader) const override = 0;

			virtual void updateShader(const Core::Shader &shader) const override = 0;

		protected:
			void updatePosition(float deltaTime);

			virtual void updateTarget(const Mouse* mouse, float deltaTime) = 0;

			virtual void zoom(const Mouse* mouse, float deltaTime) = 0;

		protected:
			CameraType _mCameraType = CameraType::NONE;

			UpdateEvent _mUpdateEvent     = UpdateEvent::NONE;
			PositionEvent _mPositionEvent = PositionEvent::NONE;

			std::shared_ptr<Object> _mVisual;
	};
}
