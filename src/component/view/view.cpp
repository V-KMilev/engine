#include "view.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

namespace Engine {
	CameraView::CameraView() : Component(ComponentType::VIEW) {}

	json CameraView::toJson() const {
		json componentJson;

		componentJson = Component::toJson();

		componentJson["position"] = {position.x, position.y, position.z};
		componentJson["target"]   = {target.x, target.y, target.z};

		componentJson["c_near"] = c_near;
		componentJson["c_far"]  = c_far;

		componentJson["front"] = {front.x, front.y, front.z};
		componentJson["right"] = {right.x, right.y, right.z};
		componentJson["up"]    = {up.x, up.y, up.z};

		componentJson["horizontalAngle"] = horizontalAngle;
		componentJson["verticalAngle"]   = verticalAngle;

		componentJson["maxUpAngle"] = maxUpAngle;

		componentJson["moveSpeed"]  = moveSpeed;
		componentJson["mouseSpeed"] = mouseSpeed;
		componentJson["zoomSpeed"]  = zoomSpeed;

		return componentJson;
	}

	void CameraView::onUpdate() {
		if (_mHasUpdate) {
			position = on_position;
			target   = on_target;

			c_near = on_c_near;
			c_far  = on_c_far;

			front = on_front;
			right = on_right;
			up    = on_up;

			horizontalAngle = on_horizontalAngle;
			verticalAngle   = on_verticalAngle;

			maxUpAngle = on_maxUpAngle;

			moveSpeed  = on_moveSpeed;
			mouseSpeed = on_mouseSpeed;
			zoomSpeed  = on_zoomSpeed;

			lookAt = glm::lookAt(position, target, up);
			updateProjection();

			// Reset
			_mHasUpdate = false;
		}
	}

	void CameraView::drawUI() const {
		std::string sPosition = "Position##Component" + std::to_string(_mID);
		std::string sTarget   = "Target##Component"   + std::to_string(_mID);

		std::string sFar  = "Far##Component"  + std::to_string(_mID);
		std::string sNear = "Near##Component" + std::to_string(_mID);

		std::string sFront = "Front##Component" + std::to_string(_mID);
		std::string sRight = "Right##Component" + std::to_string(_mID);
		std::string sUp    = "Up##Component"    + std::to_string(_mID);

		std::string sHorizontalAngle = "Horizontal Angle##Component" + std::to_string(_mID);
		std::string sVerticalAngle   = "Vertical Angle##Component"   + std::to_string(_mID);
		std::string sMaxUpAngle      = "Max Up Angle##Component"     + std::to_string(_mID);

		std::string sMoveSpeed  = "Move Speed##Component"  + std::to_string(_mID);
		std::string sMouseSpeed = "Mouse Speed##Component" + std::to_string(_mID);
		std::string sZoomSpeed  = "Zoom Speed##Component"  + std::to_string(_mID);

		ImGui::SeparatorText("View");

		if (ImGui::DragFloat3(sPosition.c_str(), &on_position[0], _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat3(sTarget.c_str(),   &on_target[0],   _mDragSpeed)) { _mHasUpdate = true; }

		ImGui::NewLine();

		if (ImGui::DragFloat(sFar.c_str(),  &on_c_far,  _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sNear.c_str(), &on_c_near, _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }

		ImGui::NewLine();

		if (ImGui::DragFloat3(sFront.c_str(), &on_front[0], _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat3(sRight.c_str(), &on_right[0], _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat3(sUp.c_str(),    &on_up[0],    _mDragSpeed)) { _mHasUpdate = true; }

		ImGui::NewLine();

		if (ImGui::DragFloat(sHorizontalAngle.c_str(), &on_horizontalAngle, _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sVerticalAngle.c_str(),   &on_verticalAngle,   _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sMaxUpAngle.c_str(),      &on_maxUpAngle,      _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }

		ImGui::NewLine();

		if (ImGui::DragFloat(sMoveSpeed.c_str(),  &on_moveSpeed,  _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sMouseSpeed.c_str(), &on_mouseSpeed, _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sZoomSpeed.c_str(),  &on_zoomSpeed,  _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }
	}

	void CameraView::updateProjection() {
		// TODO: Fix this
		return;
	}

	PerspectiveView::PerspectiveView() : CameraView() {}

	json PerspectiveView::toJson() const {
		json componentJson;

		componentJson = CameraView::toJson();

		componentJson["fov"] = fov;

		componentJson["width"]  = width;
		componentJson["height"] = height;

		componentJson["maxFOV"] = maxFOV;
		componentJson["minFOV"] = minFOV;

		return componentJson;
	}

	void PerspectiveView::onUpdate() {
		if (_mHasUpdate) {
			fov = on_fov;

			width  = on_width;
			height = on_height;

			maxFOV = on_maxFOV;
			minFOV = on_minFOV;

			CameraView::onUpdate();

			// Reset
			_mHasUpdate = false;
		}
	}

	void PerspectiveView::drawUI() const {
		std::string sFov    = "Fov##Component"     + std::to_string(_mID);
		std::string sWidth  = "Width##Component"   + std::to_string(_mID);
		std::string sHeight = "Height##Component"  + std::to_string(_mID);
		std::string sMaxFov = "Max Fov##Component" + std::to_string(_mID);
		std::string sMinFov = "Min Fov##Component" + std::to_string(_mID);

		CameraView::drawUI();

		ImGui::SeparatorText("Perspective View");

		if (ImGui::DragFloat(sFov.c_str(), &on_fov, _mDragSpeed, minFOV, maxFOV)) { _mHasUpdate = true; }

		ImGui::NewLine();

		int tmp_width  = width;
		int tmp_height = height;

		if (ImGui::DragInt(sWidth.c_str(),  &tmp_width, _mDragSpeed, 0, INT_MAX))  { on_width = tmp_width;   _mHasUpdate = true; }
		if (ImGui::DragInt(sHeight.c_str(), &tmp_height, _mDragSpeed, 0, INT_MAX)) { on_height = tmp_height; _mHasUpdate = true; }

		ImGui::NewLine();

		if (ImGui::DragFloat(sMaxFov.c_str(), &on_maxFOV, _mDragSpeed, 0, FLT_MAX))   { _mHasUpdate = true; }
		if (ImGui::DragFloat(sMinFov.c_str(),  &on_minFOV,  _mDragSpeed, 0, FLT_MAX)) { _mHasUpdate = true; }
	}

	void PerspectiveView::updateProjection() {
		projection = glm::perspective(
			glm::radians(fov),
			width / float(height),
			c_near,
			c_far
		);
	}
};
