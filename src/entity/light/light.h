#pragma once

#include "entity.h"

namespace Engine {
	class Object;

	class Mouse;
}

namespace Core {
	class Shader;
}

namespace Engine {
	enum class LightType {
		NONE = 0,
	};

	class Light : public Entity {
		public:
			Light();
			// TODO:
			// Light() = delete;
			~Light() = default;

			Light(LightType type);

			Light(const Light& other) = delete;
			Light& operator = (const Light& other) = delete;

			Light(Light && other) = delete;
			Light& operator = (Light && other) = delete;

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			void draw(const Core::Shader &shader) const override;

			void updateShader(const Core::Shader &shader) const override;

		protected:
			LightType _mLightType = LightType::NONE;

			std::shared_ptr<Object> _mVisual;
	};
};
