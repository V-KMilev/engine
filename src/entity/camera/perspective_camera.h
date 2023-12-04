#pragma once

#include "camera.h"

namespace Engine {
	class PerspectiveCamera : public Camera {
		public:
			PerspectiveCamera(unsigned int width, unsigned int height);

			void draw(const Core::Renderer& renderer, const Core::Shader& shader) const override;

			void updateShader(const Core::Shader &shader) const override;

			void UIWorld() override;

		private:
			void updateTarget(const Mouse* mouse, float deltaTime) override;
			void updateProjection() override;

			void zoom(const Mouse* mouse, float deltaTime) override;
	};
};
