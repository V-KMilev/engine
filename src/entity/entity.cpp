#include "entity.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "error_handle.h"

#include "component.h"

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
		static std::string sIsSelected = "Is Selected##Entity" + std::to_string(_mID.getID());
		static std::string sHasUpdate  = "Has Update##Entity"  + std::to_string(_mID.getID());

		static std::string sEntityType = "Entity #" + std::to_string(_mID.getID());

		if (_mType == EntityType::CAMERA) {
			sEntityType = "Camera #" + std::to_string(_mID.getID());
		}
		else if (_mType == EntityType::LIGHT) {
			sEntityType = "Light #" + std::to_string(_mID.getID());
		}
		else if (_mType == EntityType::OBJECT) {
			sEntityType = "Object #" + std::to_string(_mID.getID());
		}
		else if (_mType == EntityType::MESH) {
			sEntityType = "Mesh #" + std::to_string(_mID.getID());
		}

		ImGui::SeparatorText(sEntityType.c_str());

		ImGui::Checkbox(sIsSelected.c_str(), &_mIsSelected);

		ImGui::SameLine();

		ImGui::Checkbox(sHasUpdate.c_str(), &_mHasUpdate);

		for(const auto& component : _mComponents) {
			std::string sComponentName = component.second->getName() + "##Entity" + std::to_string(_mID.getID());

			if (ImGui::TreeNode((sComponentName.c_str()))) {
				component.second->drawUI();
				ImGui::TreePop();
			}
		}

		ImGui::NewLine();
		ImGui::SameLine(180.0f);

		if(ImGui::Button("Add Component")) {
			// TODO
		}

		ImGui::SeparatorText("");
	}
};
