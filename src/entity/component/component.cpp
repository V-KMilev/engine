#include "component.h"

#include <string>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

namespace Engine {

// TODO: Add all members as UI part

	bool ObjectWorldData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string sPosition = "Position##Object" + std::to_string(id);
		std::string sRotation = "Rotation##Object" + std::to_string(id);
		std::string sScale    = "Scale##Object"    + std::to_string(id);

		ImGui::SeparatorText("World Data");

		if (ImGui::DragFloat3(sPosition.c_str(), &position[0], _mDragSpeed)) { hasUpdate = true; }
		if (ImGui::DragFloat3(sRotation.c_str(), &rotation[0], _mDragSpeed)) { hasUpdate = true; }
		if (ImGui::DragFloat3(sScale.c_str(),    &scale[0], _mDragSpeed))    { hasUpdate = true; }

		return hasUpdate;
	}

	bool ObjectUseData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string sLinesOnly = "Lines Only##Object" + std::to_string(id);

		if(ImGui::Checkbox(sLinesOnly.c_str(), &linesOnly)) { hasUpdate = true; }

		return hasUpdate;
	}

	bool PerspectiveCameraWorldData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string sposition = "Position##Camera" + std::to_string(id);
		std::string starget   = "Target##Camera"   + std::to_string(id);
		std::string sfar      = "Far##Camera"      + std::to_string(id);
		std::string snear     = "Near##Camera"     + std::to_string(id);
		std::string sfov      = "FOV##Camera"      + std::to_string(id);
		std::string swidth    = "Width##Camera"    + std::to_string(id);
		std::string sheight   = "Height##Camera"   + std::to_string(id);

		if (ImGui::DragFloat3(sposition.c_str(), &position[0], _mDragSpeed))         { hasUpdate = true; }
		if (ImGui::DragFloat3(starget.c_str(),   &target[0], _mDragSpeed))           { hasUpdate = true; }
		if (ImGui::DragFloat(sfar.c_str(),       &c_far, _mDragSpeed, 0, FLT_MAX))   { hasUpdate = true; }
		if (ImGui::DragFloat(snear.c_str(),      &c_near, _mDragSpeed, 0, FLT_MAX))  { hasUpdate = true; }
		if (ImGui::DragFloat(sfov.c_str(),       &fov, _mDragSpeed, minFOV, maxFOV)) { hasUpdate = true; }

		int tmp_width  = width;
		int tmp_height = height;

		if (ImGui::DragInt(swidth.c_str(),  &tmp_width, _mDragSpeed, 0, INT_MAX))  { width = tmp_width;   hasUpdate = true; }
		if (ImGui::DragInt(sheight.c_str(), &tmp_height, _mDragSpeed, 0, INT_MAX)) { height = tmp_height; hasUpdate = true; }

		return hasUpdate;
	}

	bool PerspectiveCameraUseData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string smoveSpeed  = "Move Speed##Camera"  + std::to_string(id);
		std::string smouseSpeed = "Mouse Speed##Camera" + std::to_string(id);
		std::string szoomSpeed  = "Zoom Speed##Camera"  + std::to_string(id);

		if (ImGui::DragFloat(smoveSpeed.c_str(), &moveSpeed, _mDragSpeed, 0, FLT_MAX))   { hasUpdate = true; }
		if (ImGui::DragFloat(smouseSpeed.c_str(), &mouseSpeed, _mDragSpeed, 0, FLT_MAX)) { hasUpdate = true; }
		if (ImGui::DragFloat(szoomSpeed.c_str(), &zoomSpeed, _mDragSpeed, 0, FLT_MAX))   { hasUpdate = true; }

		// TODO: Add debug slot for this
		// UpdateEvent updateEvent = UpdateEvent::NONE;
		// PositionEvent positionEvent = PositionEvent::NONE;

		return hasUpdate;
	}
};