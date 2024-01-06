#include "entity.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

namespace Engine {
	Entity::Entity(EntityType type) : _mType(type) {
		_mID.updateID(_mType);
	}

	unsigned int Entity::getID() const {
		return _mID.getID();
	}

	EntityType Entity::getType() const {
		return _mType;
	}

	bool Entity::isSelected() const {
		return _mIsSelected;
	}

	bool Entity::hasUpdate() const {
		return _mHasUpdate;
	}

	void Entity::setIsSelected(bool value) {
		_mIsSelected = value;
	}

	void Entity::setHasUpdate(bool value) {
		_mHasUpdate = value;
	}

	json Entity::toJson() const {
		json entityJson;

		entityJson["id"]         = _mID.getID();
		entityJson["type"]       = static_cast<int>(_mType);
		entityJson["isSelected"] = _mIsSelected;
		entityJson["hasUpdate"]  = _mHasUpdate;

		for(const auto& component : _mComponents) {
			entityJson[component.second->getName()] = component.second->toJson();
		}

		return entityJson;
	}

	void Entity::drawUI() const {
		std::string sIsSelected = "Is Selected##Component" + std::to_string(_mID.getID());
		std::string sHasUpdate  = "Has Update##Component"  + std::to_string(_mID.getID());

		ImGui::Checkbox(sIsSelected.c_str(), &_mIsSelected);

		ImGui::SameLine();

		ImGui::Checkbox(sHasUpdate.c_str(), &_mHasUpdate);
	}
};
