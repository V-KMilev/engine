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
	class Material {
		public:
			Material();
			~Material() = default;

			Material(const Material& other) = delete;
			Material& operator = (const Material& other) = delete;

			const std::shared_ptr<Textures>& getTextures() const;
			std::shared_ptr<Textures>& getTextures();

			const std::shared_ptr<Coefficients>& getCoefficients() const;
			std::shared_ptr<Coefficients>& getCoefficients();

			void updateShader(const Core::Shader &shader) const;

			void UITextures(unsigned int id);
			bool UICoefficients(unsigned int id);

		private:
			std::shared_ptr<Textures> _mTextures;
			std::shared_ptr<Coefficients> _mCoefficients;
	};
};
