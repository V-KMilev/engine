#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "object_id.h"

namespace Core {
	class Shader;
}

namespace Engine {
	enum class CameraType {
		NONE        = 0,
		PERSPECTIVE = 1
	};

	enum class UpdateEvent {
		NONE       = 0,
		POSITION   = 1,
		TARGET     = 2,
		C_NEAR     = 3,
		C_FAR      = 4,
		MOVESPEED  = 5,
		MOUSESPEED = 6,
		FOV        = 7
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

	enum class TargetEvent {
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
			glm::vec3 position = glm::vec3(-1.0f, 0.0f, -1.0f);
			glm::vec3 target   = glm::vec3(1.0f, 0.0f, 1.0f);

			glm::mat4 lookAt = glm::lookAt(position, target, up);
	
			glm::mat4 projection = glm::mat4(1.0f);

			float c_near = 0.001f;
			float c_far  = 100.0f;

		public:
			glm::vec3 front = glm::vec3(1.0f, 0.0f,  1.0f);
			glm::vec3 right = glm::vec3(1.0f, 0.0f, -1.0f);
			glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);

			glm::vec3 mouseFront = glm::vec3(1.0f, 0.0f,  1.0f);
			glm::vec3 mouseRight = glm::vec3(1.0f, 0.0f, -1.0f);
			glm::vec3 mouseUp    = glm::vec3(0.0f, 1.0f,  0.0f);
	};

	struct UseData {
		public:
			bool isActive = false;

			float moveSpeed  = 1.0f;
			float mouseSpeed = 0.1f;

		public:
			float horizontalAngle = 0.0f;
			float verticalAngle   = 0.0f;

			float maxUpAngle = 2.0f;
	};

	class Camera {
		public:
			Camera(CameraType Type = CameraType::NONE);
			~Camera() = default;

			Camera(const Camera& other);
			Camera& operator = (const Camera& other);

			Camera(Camera && other) = delete;
			Camera& operator = (Camera && other) = delete;

			const CameraWorldData& getWorldData() const;
			CameraWorldData& getWorldData();

			const UseData& getUseData() const;
			UseData& getUseData();

			virtual void draw(const Core::Shader& shader) const = 0;

			virtual void update(
				float deltaTime,
				UpdateEvent event,
				PositionEvent pEvent = PositionEvent::NONE,
				TargetEvent tEvent   = TargetEvent::NONE
			) = 0;

		protected:
			void updatePosition(PositionEvent event, float deltaTime);
			void updateTarget(TargetEvent event, float deltaTime);

		protected:
			Core::Common::ObjectID _mID;

			CameraWorldData _mWorldData;
			UseData         _mUseData;

			CameraType _mType;
	};
}
