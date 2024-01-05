#pragma once

#include "camera.h"

namespace Engine {
	class PerspectiveCamera : public Camera {
		public:
			PerspectiveCamera(unsigned int width, unsigned int height);

			void drawUI() override;
			void draw(const Core::Shader& shader) const override;

			void updateShader(const Core::Shader &shader) const override;

		private:
			void updateTarget(const Mouse* mouse, float deltaTime) override;
			void updateProjection() override;

			void zoom(const Mouse* mouse, float deltaTime) override;
	};
};
