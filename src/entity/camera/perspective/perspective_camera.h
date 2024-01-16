#pragma once

#include <memory>

#include "camera.h"

namespace Engine {
	class PerspectiveCamera : public Camera {
		public:
			PerspectiveCamera(unsigned int width, unsigned int height);

			void draw(const Core::Shader& shader) const override;

			void updateShader(const Core::Shader &shader) const override;

		private:
			void updateTarget() override;

			void zoom() override;
	};
};
