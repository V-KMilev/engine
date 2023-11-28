#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include <memory>
#include <vector>
#include <string>

namespace Core {
	class Shader;
	class Texture;
}

namespace Engine {
	enum class TextureType {
		AMBIENT            = 0,
		DIFFUSE            = 1,
		SPECULAR           = 2,
		SPECULAR_HIGHLIGHT = 3,
		BUMP               = 4,
		DISPLACEMENT       = 5,
		ALPHA              = 6,
		REFLECTION         = 7,
		ROUGHNESS          = 8,
		METALLIC           = 9,
		SHEEN              = 10,
		EMISSIVE           = 11,
		NORMAL             = 12
	};

	struct Textures {
		public:
			std::vector<std::shared_ptr<Core::Texture>> textures{13};

			std::shared_ptr<Core::Texture>& ambient            = textures[int(TextureType::AMBIENT)];
			std::shared_ptr<Core::Texture>& diffuse            = textures[int(TextureType::DIFFUSE)];
			std::shared_ptr<Core::Texture>& specular           = textures[int(TextureType::SPECULAR)];
			std::shared_ptr<Core::Texture>& specular_highlight = textures[int(TextureType::SPECULAR_HIGHLIGHT)];
			std::shared_ptr<Core::Texture>& bump               = textures[int(TextureType::BUMP)];
			std::shared_ptr<Core::Texture>& displacement       = textures[int(TextureType::DISPLACEMENT)];
			std::shared_ptr<Core::Texture>& alpha              = textures[int(TextureType::ALPHA)];
			std::shared_ptr<Core::Texture>& reflection         = textures[int(TextureType::REFLECTION)];
			std::shared_ptr<Core::Texture>& roughness          = textures[int(TextureType::ROUGHNESS)];
			std::shared_ptr<Core::Texture>& metallic           = textures[int(TextureType::METALLIC)];
			std::shared_ptr<Core::Texture>& sheen              = textures[int(TextureType::SHEEN)];
			std::shared_ptr<Core::Texture>& emissive           = textures[int(TextureType::EMISSIVE)];
			std::shared_ptr<Core::Texture>& normal             = textures[int(TextureType::NORMAL)];
	};

	struct Coefficients {
		public:
			glm::vec3 ambient       = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 diffuse       = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 specular      = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 transmittance = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 emission      = glm::vec3(0.0f, 0.0f, 0.0f);

			float shininess = 0.0f;
			// index of refraction
			float ior       = 0.0f;

			float roughness = 0.0f;
			float metallic  = 0.0f;
			float sheen     = 0.0f;
	};

	class Material {
		public:
			Material();
			~Material() = default;

			Material(const Material& other) = delete;
			Material& operator = (const Material& other) = delete;

			const Textures& getTextures() const;
			Textures& getTextures();

			const Coefficients& getCoefficients() const;
			Coefficients& getCoefficients();

			void updateShader(const Core::Shader &shader) const;

			void drawUITextures(unsigned int id);
			bool drawUICoefficients(unsigned int id);

		private:
			Textures _mTextures;
			Coefficients _mCoefficients;
	};
};
