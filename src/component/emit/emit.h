#pragma once

#include <memory>
#include <vector>
#include <string>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "component.h"

namespace Engine {
	class Emit : public Component {
		public:
			Emit();

			const glm::vec3& getPosition() const;
			const glm::vec3& getColor() const;

			void setPosition(const glm::vec3& value, bool updateInstant = false);
			void setColor(const glm::vec3& value, bool updateInstant = false);

			virtual json toJson() const override;

			virtual void onUpdate() override;

			virtual void drawUI() const override;

		protected:
			glm::vec3 position = glm::vec3(1.0f, 1.0f, -3.0f);
			glm::vec3 color    = glm::vec3(1.0f, 1.0f, 1.0f);

			mutable glm::vec3 on_position = position;
			mutable glm::vec3 on_color   = color;
	};
};
