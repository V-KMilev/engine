#include "component.h"

#include <string>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_texture.h"

#include "utils.h"

namespace Engine {
	unsigned int Component::_mGlobalID = 1;
	float Component::_mDragSpeed = 1.0f;

	Component::Component(ComponentType type) : _mType(type) {
		_mID = _mGlobalID++;
	}

	ComponentType Component::getType() const {
		return _mType;
	}

	std::string Component::getName() const {
		return getNameFromType(_mType);
	}

	bool Component::hasUpdate() const {
		return _mHasUpdate;
	}

	void Component::setHasUpdate(bool value) {
		_mHasUpdate = value;
	}

	void Component::setDragSpeed(float speed) {
		_mDragSpeed = speed;
	}

	json Component::toJson() const {
		json componentJson;

		componentJson["id"]        = _mID;
		componentJson["type"]      = static_cast<int>(_mType);
		componentJson["hasUpdate"] = _mHasUpdate;
		componentJson["dragSpeed"] = _mDragSpeed;
		componentJson["globalID"]  = _mGlobalID;

		return componentJson;
	}

	Transform::Transform() : Component(ComponentType::TRANSFORM) {}

	json Transform::toJson() const {
		json componentJson;

		componentJson = Component::toJson();

		componentJson["position"] = {position.x, position.y, position.z};
		componentJson["rotation"] = {rotation.x, rotation.y, rotation.z};
		componentJson["scale"]    = {scale.x, scale.y, scale.z};

		return componentJson;
	}

	void Transform::onUpdate() {
		if (_mHasUpdate) {
			position = on_position;
			rotation = on_rotation;
			scale    = on_scale;

			updateModel();

			// Reset
			_mHasUpdate = false;
		}
	}

	void Transform::drawUI() const {
		std::string sPosition = "Position##Component" + std::to_string(_mID);
		std::string sRotation = "Rotation##Component" + std::to_string(_mID);
		std::string sScale    = "Scale##Component"    + std::to_string(_mID);

		ImGui::SeparatorText("Transform");

		if (ImGui::DragFloat3(sPosition.c_str(), &on_position[0], _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat3(sRotation.c_str(), &on_rotation[0], _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat3(sScale.c_str(),    &on_scale[0],    _mDragSpeed)) { _mHasUpdate = true; }
	}

	void Transform::updateModel() {
		// Reset model matrix
		model = glm::mat4(1.0f);

		model = glm::translate(model, position);

		model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, scale);
	}

	Activatable::Activatable() : Component(ComponentType::ACTIVATABLE) {}

	json Activatable::toJson() const {
		json componentJson;

		componentJson = Component::toJson();

		componentJson["isActive"] = isActive;

		return componentJson;
	}

	void Activatable::onUpdate() {
		if (_mHasUpdate) {
			isActive = on_isActive;

			// Reset
			_mHasUpdate = false;
		}
	}

	void Activatable::drawUI() const {
		std::string sIsActive = "Is Active##Component"  + std::to_string(_mID);

		ImGui::SeparatorText("Activatable");

		if (ImGui::Checkbox(sIsActive.c_str(), &on_isActive)) { _mHasUpdate = true; }
	}

	LinesOnly::LinesOnly() : Component(ComponentType::LINESONLY) {}

	json LinesOnly::toJson() const {
		json componentJson;

		componentJson = Component::toJson();

		componentJson["linesOnly"] = linesOnly;

		return componentJson;
	}

	void LinesOnly::onUpdate() {
		if (_mHasUpdate) {
			linesOnly = on_linesOnly;

			// Reset
			_mHasUpdate = false;
		}
	}

	void LinesOnly::drawUI() const {
		std::string sLinesOnly = "Lines Only##Component" + std::to_string(_mID);

		ImGui::SeparatorText("LinesOnly");

		if (ImGui::Checkbox(sLinesOnly.c_str(), &on_linesOnly)) { _mHasUpdate = true; }
	}
}
