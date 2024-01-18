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

	static std::string getNameFromType(TextureType type) {
		switch (type) {
			case TextureType::AMBIENT:            return "Ambient";
			case TextureType::DIFFUSE:            return "Diffuse";
			case TextureType::SPECULAR:           return "Specular";
			case TextureType::SPECULAR_HIGHLIGHT: return "Specular_highlight";
			case TextureType::BUMP:               return "Bump";
			case TextureType::DISPLACEMENT:       return "Displacement";
			case TextureType::ALPHA:              return "Alpha";
			case TextureType::REFLECTION:         return "Reflection";
			case TextureType::ROUGHNESS:          return "Roughness";
			case TextureType::METALLIC:           return "Metallic";
			case TextureType::SHEEN:              return "Sheen";
			case TextureType::EMISSIVE:           return "Emissive";
			case TextureType::NORMAL:             return "Normal";
			default:                              return "Invalid";
		}
	}

	class Textures {
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

	class Coefficients {
		public:
			glm::vec3 ambient       = glm::vec3(1.0f, 1.0f, 1.0f);
			glm::vec3 diffuse       = glm::vec3(1.0f, 1.0f, 1.0f);
			glm::vec3 specular      = glm::vec3(1.0f, 1.0f, 1.0f);
			glm::vec3 transmittance = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 emission      = glm::vec3(0.0f, 0.0f, 0.0f);

			float shininess = 32.0f;
			// index of refraction
			float ior       = 0.0f;

			float roughness = 0.0f;
			float metallic  = 0.0f;
			float sheen     = 0.0f;
	};

	class Material : public Component {
		public:
			Material();
			~Material() = default;

			Material(const Material& other) = default;
			Material& operator = (const Material& other) = default;

			Material(Material && other) = delete;
			Material& operator = (Material && other) = delete;

			const std::vector<std::shared_ptr<Core::Texture>>& getTextures() const;
			std::vector<std::shared_ptr<Core::Texture>>& getTextures();

			const glm::vec3& getAmbient() const;
			const glm::vec3& getDiffuse() const;
			const glm::vec3& getSpecular() const;
			const glm::vec3& getTransmittance() const;
			const glm::vec3& getEmission() const;

			float getShininess() const;
			float getIor() const;

			float getRoughness() const;
			float getMetallic() const;
			float getSheen() const;

			void setAmbient(const glm::vec3& value, bool updateInstant = false);
			void setDiffuse(const glm::vec3& value, bool updateInstant = false);
			void setSpecular(const glm::vec3& value, bool updateInstant = false);
			void setTransmittance(const glm::vec3& value, bool updateInstant = false);
			void setEmission(const glm::vec3& value, bool updateInstant = false);

			void setShininess(float value, bool updateInstant = false);
			void setIor(float value, bool updateInstant = false);

			void setRoughness(float value, bool updateInstant = false);
			void setMetallic(float value, bool updateInstant = false);
			void setSheen(float value, bool updateInstant = false);

			json toJson() const override;

			void onUpdate() override;

			void drawUI() const override;

			void updateShader(const Core::Shader &shader) const;

		private:
			std::shared_ptr<Textures> textures;
			std::shared_ptr<Coefficients> coefficients;

			mutable std::shared_ptr<Textures> on_textures;
			mutable std::shared_ptr<Coefficients> on_coefficients;
	};
};
