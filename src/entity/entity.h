#pragma once

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
		MESH   = 2,
		CAMERA = 3
	};

	// Base WorldData struct
	struct WorldData {};

	// Base UseData struct
	struct UseData {
		public:
			bool isActive  = false;
			bool hasUpdate = false;
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

			virtual const WorldData& getWorldData() const = 0;
			virtual WorldData& getWorldData() = 0;

			virtual const UseData& getUseData() const = 0;
			virtual UseData& getUseData() = 0;

			virtual void onUpdate(const Mouse* mouse, float deltaTime) = 0;

			virtual void draw(const Core::Renderer &renderer, const Core::Shader &shader) const = 0;

			virtual void drawUIParams() = 0;

		protected:
			virtual void updateShader(const Core::Shader &shader) const = 0;

		protected:
			EntityID _mID;

			EntityType _mType;

			WorldData _mWorldData;
			UseData _mUseData;
	};
};
