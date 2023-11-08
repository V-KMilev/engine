#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "entity.h"

namespace Engine {
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

	struct CameraWorldData : WorldData {
		public:
			glm::vec3 position = glm::vec3(0.0f, 1.0f, -5.0f);
			glm::vec3 target   = glm::vec3(0.0f, 1.0f, 1.0f);

			glm::mat4 lookAt = glm::lookAt(position, target, up);
	
			glm::mat4 projection = glm::mat4(1.0f);

			float c_near = 0.01f;
			float c_far  = 100.0f;

		public:
			glm::vec3 front = glm::vec3(1.0f, 0.0f,  1.0f);
			glm::vec3 right = glm::vec3(1.0f, 0.0f, -1.0f);
			glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);

		public:
			float horizontalAngle = 0.0f;
			float verticalAngle   = 0.0f;

			float maxUpAngle = 1.70f;
	};

	struct CameraUseData : UseData {
		public:
			float moveSpeed  = 10.0f;
			float mouseSpeed = 0.05f;

			float zoomSpeed  = 500.0f;

		public:
			UpdateEvent updateEvent = UpdateEvent::NONE;
			PositionEvent positionEvent = PositionEvent::NONE;
	};

	class Camera : public Entity {
		public:
			Camera(CameraType Type = CameraType::NONE);

			CameraType getCameraTpye() const;

			const CameraWorldData& getWorldData() const override;
			CameraWorldData& getWorldData() override;

			const CameraUseData& getUseData() const override;
			CameraUseData& getUseData() override;

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			virtual void draw(const Core::Renderer &renderer, const Core::Shader &shader) const override = 0;

			virtual void drawUIParams() override = 0;
		protected:
			void updatePosition(float deltaTime);
			void updateLookAt();

			virtual void updateShader(const Core::Shader &shader) const override = 0;

			virtual void updateTarget(const Mouse* mouse, float deltaTime) = 0;
			virtual void updateProjection() = 0;

			virtual void zoom(const Mouse* mouse, float deltaTime) = 0;

		protected:
			CameraType _mCameraType;

			CameraWorldData _mCameraWorldData;
			CameraUseData _mCameraUseData;
	};
}
