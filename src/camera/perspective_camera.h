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

			unsigned int getWidth() const;
			unsigned int& getWidth();

			unsigned int getHeight() const;
			unsigned int& getHeight();

			void draw(const Core::Shader& shader) const override;

		private:
			void updateTarget(const Mouse* mouse, float deltaTime) override;
			void updateProjection() override;

			void zoom(const Mouse* mouse, float deltaTime) override;

		private:
			float _mFov = 60.0f;

			unsigned int _mHeight = 0;
			unsigned int _mWidth  = 0;
	};
};
