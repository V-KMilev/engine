#pragma once

#include <memory>
#include <vector>
#include <string>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "json.hpp"

namespace Core {
	class Texture;
};

using json = nlohmann::json;

namespace Engine {
	enum class ComponentType {
		NONE        = 0,
		TRANSFORM   = 1,
		MATERIAL    = 2,
		ACTIVATABLE = 3,
		LINESONLY   = 4,
		VIEW        = 5
	};

	static std::string getNameFromType(ComponentType type) {
		switch(type) {
			case ComponentType::TRANSFORM:   return "Transform";
			case ComponentType::MATERIAL:    return "Material";
			case ComponentType::ACTIVATABLE: return "Activatable";
			case ComponentType::LINESONLY:   return "Linesonly";
			default:                         return "Invalid";
		}
	}

	class Component {
		public:
			Component(ComponentType type);
			virtual ~Component() = default;

			ComponentType getType() const;
			std::string getName() const;

			bool hasUpdate() const;

			void setHasUpdate(bool value);

			void setDragSpeed(float speed);

			virtual json toJson() const;
			// virtual json fromJson(const json& content) const = 0;

			virtual void onUpdate() = 0;

			virtual void drawUI() const = 0;

		protected:
			unsigned int _mID = 0;
			ComponentType _mType = ComponentType::NONE;

			mutable bool _mHasUpdate = false;

			static float _mDragSpeed;
			static unsigned int _mGlobalID;
	};

	class Transform : public Component {
		public:
			Transform();

			json toJson() const override;

			void onUpdate() override;

			void drawUI() const override;

		private:
			void updateModel();

		public:
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);

			glm::mat4 model = glm::mat4(1.0f);

		public:
			mutable glm::vec3 on_position = position;
			mutable glm::vec3 on_rotation = rotation;
			mutable glm::vec3 on_scale    = scale;
	};

	class Activatable : public Component {
		public:
			Activatable();

			json toJson() const override;

			void onUpdate() override;

			void drawUI() const override;

		public:
			bool isActive = false;

		public:
			mutable bool on_isActive = isActive;
	};

	class LinesOnly : public Component {
		public:
			LinesOnly();

			json toJson() const override;

			void onUpdate() override;

			void drawUI() const override;

		public:
			bool linesOnly = false;
		
		public:
			mutable bool on_linesOnly = linesOnly;
	};
};
