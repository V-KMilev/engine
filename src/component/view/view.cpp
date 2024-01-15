#include "view.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "input_manager.h"

namespace Engine {
	CameraView::CameraView() : Component(ComponentType::VIEW) {}

	const glm::vec3& CameraView::getPosition() const { return position; }
	const glm::vec3& CameraView::getTarget() const { return target; }
	const glm::mat4& CameraView::getLookAt() const { return lookAt; }
	const glm::mat4& CameraView::getProjection() const { return projection; }

	float CameraView::getNear() const { return c_near; }
	float CameraView::getFar() const { return c_far; }

	const glm::vec3& CameraView::getFront() const { return front; }
	const glm::vec3& CameraView::getRight() const { return right; }
	const glm::vec3& CameraView::getUp() const { return up; }

	float CameraView::getHorizontalAngle() const { return horizontalAngle; }
	float CameraView::getVerticalAngle() const { return verticalAngle; }
	float CameraView::getMaxUpAngle() const { return maxUpAngle; }

	float CameraView::getMoveSpeed() const { return moveSpeed; };
	float CameraView::getMouseSpeed() const { return mouseSpeed; };
	float CameraView::getZoomSpeed() const { return zoomSpeed; };

	void CameraView::setPosition(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(position, on_position, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setTarget(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(target, on_target, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setNear(float value, bool updateInstant) {
		setValue<float>(c_near, on_c_near, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setFar(float value, bool updateInstant) {
		setValue<float>(c_far, on_c_far, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setFront(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(front, on_front, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setRight(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(right, on_right, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setUp(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(up, on_up, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setHorizontalAngle(float value, bool updateInstant) {
		setValue<float>(horizontalAngle, on_horizontalAngle, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setVerticalAngle(float value, bool updateInstant) {
		setValue<float>(verticalAngle, on_verticalAngle, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setMaxUpAngle(float value, bool updateInstant) {
		setValue<float>(maxUpAngle, on_maxUpAngle, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setMoveSpeed(float value, bool updateInstant) {
		setValue<float>(moveSpeed, on_moveSpeed, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setMouseSpeed(float value, bool updateInstant) {
		setValue<float>(mouseSpeed, on_mouseSpeed, _mHasUpdate, value, updateInstant);
	}

	void CameraView::setZoomSpeed(float value, bool updateInstant) {
		setValue<float>(zoomSpeed, on_zoomSpeed, _mHasUpdate, value, updateInstant);
	}

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

	float PerspectiveView::getFov() const { return fov; }

	unsigned int PerspectiveView::getWidth() const { return width; }
	unsigned int PerspectiveView::getHeight() const { return height; }

	float PerspectiveView::getMaxFOV() const { return fov; }
	float PerspectiveView::getMinFOV() const { return fov; }

	void PerspectiveView::setFov(float value, bool updateInstant) {
		setValue<float>(fov, on_fov, _mHasUpdate, value, updateInstant);
	}

	void PerspectiveView::setWidth(unsigned int value, bool updateInstant) {
		setValue<unsigned int>(width, on_width, _mHasUpdate, value, updateInstant);
	}

	void PerspectiveView::setHeight(unsigned int value, bool updateInstant) {
		setValue<unsigned int>(height, height, _mHasUpdate, value, updateInstant);
	}

	void PerspectiveView::setMaxFOV(float value, bool updateInstant) {
		setValue<float>(maxFOV, on_maxFOV, _mHasUpdate, value, updateInstant);
	}

	void PerspectiveView::setMinFOV(float value, bool updateInstant) {
		setValue<float>(minFOV, on_minFOV, _mHasUpdate, value, updateInstant);
	}

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

	void PerspectiveView::calculateInitView(unsigned int width, unsigned int height) {
		on_width = width;
		on_height = height;

		projection = glm::perspective(
			glm::radians(fov),
			on_width / float(on_height),
			c_near,
			c_far
		);

		// Calculate initial camera orientation
		on_front = glm::normalize(on_target - on_position);
		on_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), on_front));
		on_up = glm::cross(on_front, on_right);

		// Ensure that the up vector is perpendicular to the direction vector
		if (glm::length(on_up) < 0.9f) {
			on_right = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), on_front));
			on_up    = glm::cross(on_front, on_right);
		}

		_mHasUpdate = true;
	}

	void PerspectiveView::updateTarget(const Mouse* mouse) {
		// Calculate the mouse movement angles
		float horizontalDelta = mouseSpeed * (width / 2.0f - mouse->x);
		float verticalDelta   = mouseSpeed * (height / 2.0f - mouse->y);

		// Update the horizontal & vertical angle
		on_horizontalAngle += horizontalDelta;
		on_verticalAngle += verticalDelta;

		// Clamp the vertical angle
		on_verticalAngle = glm::clamp(on_verticalAngle, -maxUpAngle, maxUpAngle);

		// Calculate the new front direction
		glm::vec3 front(
			cos(on_verticalAngle) * sin(on_horizontalAngle),
			sin(on_verticalAngle),
			cos(on_verticalAngle) * cos(on_horizontalAngle)
		);

		// front * 2.0f so when you try to move you dont start spining
		on_target = on_position + (front * 2.0f);

		_mHasUpdate = true;
	}

	void PerspectiveView::zoom(const Mouse* mouse) {
		if (mouse->hasUpdate) {
			on_fov -= zoomSpeed * mouse->scrollY;
		}

		if (fov > maxFOV) {
			on_fov = maxFOV;
		}
		else if (fov < minFOV) {
			on_fov = minFOV;
		}

		_mHasUpdate = true;
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
