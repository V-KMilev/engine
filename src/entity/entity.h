#pragma once

#include <memory>

#include "component.h"
#include "entity_id.h"

namespace Core {
	class Renderer;
	class Shader;
}

namespace Engine {
	struct Mouse;
};

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

			EntityType getTpye() const;

			virtual const std::shared_ptr<WorldData>& getWorldData() const;
			virtual std::shared_ptr<WorldData>& getWorldData();

			virtual const std::shared_ptr<UseData>& getUseData() const;
			virtual std::shared_ptr<UseData>& getUseData();

			virtual void onUpdate(const Mouse* mouse, float deltaTime) = 0;

			virtual void draw(const Core::Renderer &renderer, const Core::Shader &shader) const = 0;

			virtual void updateShader(const Core::Shader &shader) const = 0;

			virtual void UIWorld() = 0;

		protected:
			EntityID _mID;

			EntityType _mType;

			std::shared_ptr<WorldData> _mWorldData;
			std::shared_ptr<UseData> _mUseData;
	};
};
