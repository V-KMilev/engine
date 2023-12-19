#pragma once

#include <memory>

#include "component.h"
#include "entity_id.h"

#include "json.hpp"

namespace Core {
	class Renderer;
	class Shader;
}

namespace Engine {
	struct Mouse;
};

using json = nlohmann::json;

namespace Engine {
	enum class EntityType {
		NONE   = 0,
		OBJECT = 1,
		CAMERA = 2
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

			virtual const std::shared_ptr<Transform>& getTransform() const;
			virtual std::shared_ptr<Transform>& getTransform();

			virtual const std::shared_ptr<InteractionState>& getInteractionState() const;
			virtual std::shared_ptr<InteractionState>& getInteractionState();

			json toJson() const;

			virtual void onUpdate(const Mouse* mouse, float deltaTime) = 0;

			virtual void draw(const Core::Renderer &renderer, const Core::Shader &shader) const = 0;

			virtual void updateShader(const Core::Shader &shader) const = 0;

			virtual void UIWorld() = 0;

		protected:
			EntityID _mID;

			EntityType _mType;

			std::shared_ptr<Transform> _mTransform;
			std::shared_ptr<InteractionState> _mInteractionState;
	};
};
