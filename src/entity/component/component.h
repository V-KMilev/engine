#pragma once

#include <memory>
#include <vector>
#include <string>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "json.hpp"

#include "ui_element.h"

namespace Core {
	class Texture;
};

using json = nlohmann::json;

namespace Engine {
	// Base Transform struct
	class Transform : public UIElement {
		public:
			virtual json toJson() const = 0;

			virtual bool drawUI(unsigned int id) override = 0;
	};

	// Base InteractionState struct
	class InteractionState : public UIElement {
		public:
			virtual json toJson() const = 0;

			virtual bool drawUI(unsigned int id) override = 0;

		public:
			bool isActive   = false;
			bool isSelected = false;
			bool hasUpdate  = false;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class ObjectTransform : public Transform {
		public:
			json toJson() const override;

			bool drawUI(unsigned int id) override;

		public:
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			// roll, pitch, yaw
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

			glm::mat4 model = glm::mat4(1.0f);
	};

	class ObjectInteractionState : public InteractionState {
		public:
			json toJson() const override;

			bool drawUI(unsigned int id) override;

		public:
			bool linesOnly = false;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	enum class UpdateEvent {
		NONE       = 0,
		POSITION   = 1,
		TARGET     = 2,
		FOV        = 3
	};

	enum class PositionEvent {
		NONE = 0,
		POSX = 1,
		NEGX = 2,
		POSY = 3,
		NEGY = 4,
		POSZ = 5,
		NEGZ = 6
	};

	class CameraTransform : public Transform {
		public:
			virtual json toJson() const override = 0;

			virtual bool drawUI(unsigned int id) override = 0;

		public:
			glm::vec3 position = glm::vec3(0.0f, 1.0f, -7.0f);
			glm::vec3 target   = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::mat4 lookAt = glm::lookAt(position, target, up);
	
			glm::mat4 projection = glm::mat4(1.0f);

			float c_near = 0.01f;
			float c_far  = 100.0f;

		public:
			glm::vec3 front = glm::vec3(1.0f, 0.0f,  1.0f);
			glm::vec3 right = glm::vec3(1.0f, 0.0f, -1.0f);
			glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);

		public:
			float horizontalAngle = 0.0f;
			float verticalAngle   = 0.0f;

			float maxUpAngle = 1.57f;
	};

	class CameraInteractionState : public InteractionState {
		public:
			virtual json toJson() const override = 0;

			virtual bool drawUI(unsigned int id) override = 0;

		public:
			float moveSpeed  = 10.0f;
			float mouseSpeed = 0.05f;

			float zoomSpeed  = 500.0f;

		public:
			UpdateEvent updateEvent = UpdateEvent::NONE;
			PositionEvent positionEvent = PositionEvent::NONE;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class PerspectiveCameraTransform : public CameraTransform {
		public:
			json toJson() const override;

			bool drawUI(unsigned int id) override;

		public:
			// field of view
			float fov = 45.0f;

			unsigned int width  = 0;
			unsigned int height = 0;

		public:
			float maxFOV = 175.0f;
			float minFOV = 5.0f;
	};

	class PerspectiveCameraInteractionState : public CameraInteractionState {
		public:
			json toJson() const override;

			bool drawUI(unsigned int id) override;

		public:
	};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	static std::string TextureTypeToString(TextureType type) {
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
			default:                              return "NONE";
		}
	}

	class Textures : public UIElement {
		public:
			json toJson() const;

			bool drawUI(unsigned int id) override;

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

	class Coefficients : public UIElement {
		public:
			json toJson() const;

			bool drawUI(unsigned int id) override;

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
};
