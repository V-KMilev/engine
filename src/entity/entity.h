#pragma once

#include <memory>

#include "error_handle.h"

#include "component.h"
#include "entity_id.h"

#include "json.hpp"

namespace Core {
	class Shader;
}

namespace Engine {
	struct Mouse;
};

using json = nlohmann::json;

namespace Engine {
	enum class EntityType {
		NONE   = 0,
		MESH   = 1,
		OBJECT = 2,
		CAMERA = 3,
		LIGHT  = 4
	};

	class Entity {
		public:
			Entity(EntityType type);
			virtual ~Entity() = default;

			Entity(const Entity& other) = delete;
			Entity& operator = (const Entity& other) = delete;

			Entity(Entity && other) = delete;
			Entity& operator = (Entity && other) = delete;

			unsigned int getID() const;
			EntityType getType() const;

			bool isSelected() const;
			bool hasUpdate() const;

			void setIsSelected(bool value);
			void setHasUpdate(bool value);

			template<typename T>
			std::shared_ptr<T> Entity::addComponent(std::shared_ptr<T> newComponent = nullptr) {
				if (newComponent) {
					// Initialize the component with the provided newComponent
					auto component = std::make_shared<T>(*newComponent);

					_mComponents[typeid(T).hash_code()] = component;

					return component;
				} else {
					// Create a new component without specific initialization
					auto component = std::make_shared<T>();

					_mComponents[typeid(T).hash_code()] = component;

					return component;
				}
			}

			template<typename T>
			std::shared_ptr<T> Entity::getComponent() {
				auto it = _mComponents.find(typeid(T).hash_code());

				if (it != _mComponents.end()) {
					return std::static_pointer_cast<T>(it->second);
				}

				return nullptr;
			}

			template<typename T>
			std::shared_ptr<T> Entity::getComponent() const {
				auto it = _mComponents.find(typeid(T).hash_code());

				if (it != _mComponents.end()) {
					return std::static_pointer_cast<T>(it->second);
				}

				return nullptr;
			}

			json toJson() const;

			virtual void onUpdate(const Mouse* mouse, float deltaTime) = 0;

			virtual void drawUI() const;
			virtual void draw(const Core::Shader &shader) const = 0;

			virtual void updateShader(const Core::Shader &shader) const = 0;

		protected:
			EntityID _mID;
			EntityType _mType = EntityType::NONE;

			mutable bool _mIsSelected = false;
			mutable bool _mHasUpdate  = false;

			std::unordered_map<size_t, std::shared_ptr<Component>> _mComponents;
	};
};
