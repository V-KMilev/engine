#pragma once

#include "camera.h"

namespace Engine {
	class PerspectiveCamera : public Camera {
		public:
			PerspectiveCamera(unsigned int width, unsigned int height);
			~PerspectiveCamera() = default;

			PerspectiveCamera(const PerspectiveCamera& other);
			PerspectiveCamera& operator = (const PerspectiveCamera& other);

			PerspectiveCamera(Camera && other) = delete;
			PerspectiveCamera& operator = (PerspectiveCamera && other) = delete;

			float getFov() const;
			float& getFov();

			void draw(const Core::Shader& shader) const override;

			void update(
				float deltaTime,
				UpdateEvent event,
				PositionEvent pEvent = PositionEvent::NONE,
				const Mouse* mouse = nullptr,
				unsigned int width = 0,
				unsigned int hight = 0
			) override;

		private:
			void updateProjection();

		private:
			float _mFov = 60.0f;

			unsigned int _mHeight = 0;
			unsigned int _mWidth  = 0;
	};
};
