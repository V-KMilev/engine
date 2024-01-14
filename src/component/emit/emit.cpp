#include "emit.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "input_manager.h"

namespace Engine {
	Emit::Emit() : Component(ComponentType::EMIT) {}

	const glm::vec3& Emit::getPosition() const { return position; }
	const glm::vec3& Emit::getColor() const { return color; }

	void Emit::setPosition(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(position, on_position, _mHasUpdate, value, updateInstant);
	}

	void Emit::setColor(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(color, on_color, _mHasUpdate, value, updateInstant);
	}

	json Emit::toJson() const {
		json componentJson;

		componentJson = Component::toJson();

		componentJson["position"] = {position.x, position.y, position.z};
		componentJson["color"]    = {color.x, color.y, color.z};

		return componentJson;
	}

	void Emit::onUpdate() {
		if (_mHasUpdate) {
			position = on_position;
			color    = on_color;

			// Reset
			_mHasUpdate = false;
		}
	}

	void Emit::drawUI() const {
		std::string sPosition = "Position##Component" + std::to_string(_mID);
		std::string sColor    = "Color##Component"    + std::to_string(_mID);

		if (ImGui::DragFloat3(sPosition.c_str(), &on_position[0], _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::ColorEdit3(sColor.c_str(), &on_color[0]))                    { _mHasUpdate = true; }
	}
};
