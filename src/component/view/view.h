#pragma once

#include <memory>
#include <vector>
#include <string>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "component.h"

namespace Core {
	class Shader;
	class Texture;
}

namespace Engine {
	class CameraView : public Component {
		public:
			CameraView();

			virtual json toJson() const override;

			virtual void onUpdate() override;

			virtual void drawUI() const override;

		protected:
			virtual void updateProjection();

		public:
			glm::vec3 position = glm::vec3(0.0f, 1.0f, -7.0f);
			glm::vec3 target   = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::mat4 lookAt     = glm::lookAt(position, target, up);
			glm::mat4 projection = glm::mat4(1.0f);

			float c_near = 0.01f;
			float c_far  = 100.0f;

			glm::vec3 front = glm::vec3(1.0f, 0.0f,  1.0f);
			glm::vec3 right = glm::vec3(1.0f, 0.0f, -1.0f);
			glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);

			float horizontalAngle = 0.0f;
			float verticalAngle   = 0.0f;

			float maxUpAngle = 1.57f;

			float moveSpeed  = 10.0f;
			float mouseSpeed = 0.05f;
			float zoomSpeed  = 500.0f;

		public:
			mutable glm::vec3 on_position = position;
			mutable glm::vec3 on_target   = target;

			mutable float on_c_near = c_near;
			mutable float on_c_far  = c_far;

			mutable glm::vec3 on_front = front;
			mutable glm::vec3 on_right = right;
			mutable glm::vec3 on_up    = up;

			mutable float on_horizontalAngle = horizontalAngle;
			mutable float on_verticalAngle   = verticalAngle;

			mutable float on_maxUpAngle = maxUpAngle;

			mutable float on_moveSpeed  = moveSpeed;
			mutable float on_mouseSpeed = mouseSpeed;
			mutable float on_zoomSpeed  = zoomSpeed;
	};

	class PerspectiveView : public CameraView {
		public:
			PerspectiveView();

			json toJson() const override;

			void onUpdate() override;

			void drawUI() const override;

		private:
			void updateProjection() override;

		public:
			float fov = 45.0f;

			unsigned int width  = 0;
			unsigned int height = 0;

			float maxFOV = 175.0f;
			float minFOV = 5.0f;

		public:
			mutable float on_fov = fov;

			mutable unsigned int on_width  = width;
			mutable unsigned int on_height = height;

			mutable float on_maxFOV = maxFOV;
			mutable float on_minFOV = minFOV;
	};
};
