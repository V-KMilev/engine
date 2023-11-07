#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "entity_id.h"

namespace Core {
	class Shader;
}

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
		FOV        = 3,
		UI         = 4
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

	struct CameraWorldData {
		public:
			void updateLookAt();

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

			float maxUpAngle = 2.0f;
	};

	struct UseData {
		public:
			bool isActive = false;
			bool hasUpdate = false;

			float moveSpeed  = 10.0f;
			float mouseSpeed = 0.05f;
	};

	class Camera {
		public:
			Camera(CameraType Type = CameraType::NONE);
			~Camera() = default;

			Camera(const Camera& other);
			Camera& operator = (const Camera& other);

			Camera(Camera && other) = delete;
			Camera& operator = (Camera && other) = delete;

			unsigned int getID() const;
			CameraType getTpye() const;

			const CameraWorldData& getWorldData() const;
			CameraWorldData& getWorldData();

			const UseData& getUseData() const;
			UseData& getUseData();

			virtual void draw(const Core::Shader& shader) const = 0;

			virtual void update(
				UpdateEvent event    = UpdateEvent::NONE,
				PositionEvent pEvent = PositionEvent::NONE,
				float deltaTime      = 0.0f,
				const Mouse* mouse   = nullptr
			);

		protected:
			void updatePosition(PositionEvent event, float deltaTime);
			void updateLookAt();

			virtual void updateTarget(const Mouse* mouse, float deltaTime) = 0;
			virtual void updateProjection() = 0;

			virtual void zoom(const Mouse* mouse, float deltaTime) = 0;

		protected:
			Core::EntityID _mID;

			CameraWorldData _mWorldData;
			UseData         _mUseData;

			CameraType _mType;
	};
}
