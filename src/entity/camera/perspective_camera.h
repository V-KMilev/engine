#pragma once

#include "camera.h"

namespace Engine {
	class PerspectiveCamera : public Camera {
		public:
			PerspectiveCamera(unsigned int width, unsigned int height);

			float getFov() const;
			float& getFov();

			unsigned int getWidth() const;
			unsigned int& getWidth();

			unsigned int getHeight() const;
			unsigned int& getHeight();

			void draw(const Core::Renderer& renderer, const Core::Shader& shader) const override;

			void drawUIParams() override;

		private:
			void updateShader(const Core::Shader &shader) const override;

			void updateTarget(const Mouse* mouse, float deltaTime) override;
			void updateProjection() override;

			void zoom(const Mouse* mouse, float deltaTime) override;

		private:
			float _mFov = 60.0f;

			unsigned int _mHeight = 0;
			unsigned int _mWidth  = 0;
	};
};
