#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "ui_element.h"

namespace Engine {
	// Base WorldData struct
	class WorldData : public UIElement {
		public:
			virtual bool drawUI(unsigned int id) override = 0;
	};

	// Base UseData struct
	class UseData : public UIElement {
		public:
			virtual bool drawUI(unsigned int id) override = 0;

		public:
			bool isActive   = false;
			bool isSelected = false;
			bool hasUpdate  = false;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class ObjectWorldData : public WorldData {
		public:
			bool drawUI(unsigned int id) override;

		public:
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			// roll, pitch, yaw
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

			glm::mat4 model = glm::mat4(1.0f);
	};

	class ObjectUseData : public UseData {
		public:
			bool drawUI(unsigned int id) override;

		public:
			bool linesOnly = false;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	class CameraWorldData : public WorldData {
		public:
			virtual bool drawUI(unsigned int id) override = 0;

		public:
			glm::vec3 position = glm::vec3(0.0f, 1.0f, -7.0f);
			glm::vec3 target   = glm::vec3(0.0f, 1.0f, 0.0f);

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

			float maxUpAngle = 1.57f;
	};

	class CameraUseData : public UseData {
		public:
			virtual bool drawUI(unsigned int id) override = 0;

		public:
			float moveSpeed  = 10.0f;
			float mouseSpeed = 0.05f;

			float zoomSpeed  = 500.0f;

		public:
			UpdateEvent updateEvent = UpdateEvent::NONE;
			PositionEvent positionEvent = PositionEvent::NONE;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class PerspectiveCameraWorldData : public CameraWorldData {
		public:
			bool drawUI(unsigned int id) override;

		public:
			// field of view
			float fov = 45.0f;

			unsigned int width  = 0;
			unsigned int height = 0;

		public:
			float maxFOV = 175.0f;
			float minFOV = 5.0f;
	};

	class PerspectiveCameraUseData : public CameraUseData {
		public:
			bool drawUI(unsigned int id) override;

		public:
	};
};
