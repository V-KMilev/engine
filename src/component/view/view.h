#pragma once

#include <memory>
#include <vector>
#include <string>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "component.h"

namespace Engine {
	class Mouse;
}

namespace Engine {
	class CameraView : public Component {
		public:
			CameraView();

			const glm::vec3& getPosition() const;
			const glm::vec3& getTarget() const;
			const glm::mat4& getLookAt() const;
			const glm::mat4& getProjection() const;

			float getNear() const;
			float getFar() const;

			const glm::vec3& getFront() const;
			const glm::vec3& getRight() const;
			const glm::vec3& getUp() const;

			float getHorizontalAngle() const;
			float getVerticalAngle() const;
			float getMaxUpAngle() const;

			float getMoveSpeed() const;
			float getMouseSpeed() const;
			float getZoomSpeed() const;

			void setPosition(const glm::vec3& value, bool updateInstant = false);
			void setTarget(const glm::vec3& value, bool updateInstant = false);

			void setNear(float value, bool updateInstant = false);
			void setFar(float value, bool updateInstant = false);

			void setFront(const glm::vec3& value, bool updateInstant = false);
			void setRight(const glm::vec3& value, bool updateInstant = false);
			void setUp(const glm::vec3& value, bool updateInstant = false);

			void setHorizontalAngle(float value, bool updateInstant = false);
			void setVerticalAngle(float value, bool updateInstant = false);
			void setMaxUpAngle(float value, bool updateInstant = false);

			void setMoveSpeed(float value, bool updateInstant = false);
			void setMouseSpeed(float value, bool updateInstant = false);
			void setZoomSpeed(float value, bool updateInstant = false);

			virtual json toJson() const override;

			virtual void onUpdate() override;

			virtual void drawUI() const override;

			virtual void calculateInitView(unsigned int width, unsigned int height) = 0;

		protected:
			virtual void updateProjection();

		protected:
			glm::vec3 position = glm::vec3(7.0f, 1.0f, 0.0f);
			glm::vec3 target   = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::mat4 lookAt     = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);

			float c_near = 0.01f;
			float c_far  = 100.0f;

			glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 up    = glm::vec3(0.0f, 0.0f, 0.0f);

			float horizontalAngle = 0.0f;
			float verticalAngle   = 0.0f;

			float maxUpAngle = 1.57f;

			float moveSpeed  = 10.0f;
			float mouseSpeed = 0.05f;
			float zoomSpeed  = 500.0f;

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

			float getFov() const;

			unsigned int getWidth() const;
			unsigned int getHeight() const;

			float getMaxFOV() const;
			float getMinFOV() const;

			void setFov(float value, bool updateInstant = false);

			void setWidth(unsigned int value, bool updateInstant = false);
			void setHeight(unsigned int value, bool updateInstant = false);

			void setMaxFOV(float value, bool updateInstant = false);
			void setMinFOV(float value, bool updateInstant = false);

			json toJson() const override;

			void onUpdate() override;

			void drawUI() const override;

			void calculateInitView(unsigned int width, unsigned int height) override;

			void updateTarget(const Mouse* mouse, float deltaTime);
			void zoom(const Mouse* mouse, float deltaTime);

		private:
			void updateProjection() override;

		private:
			float fov = 45.0f;

			unsigned int width  = 0;
			unsigned int height = 0;

			float maxFOV = 175.0f;
			float minFOV = 5.0f;

			mutable float on_fov = fov;

			mutable unsigned int on_width  = width;
			mutable unsigned int on_height = height;

			mutable float on_maxFOV = maxFOV;
			mutable float on_minFOV = minFOV;
	};
};
