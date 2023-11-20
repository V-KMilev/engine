#include "perspective_camera.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_shader.h"

#include "input_handler.h"

namespace Engine {
	PerspectiveCamera::PerspectiveCamera(unsigned int width, unsigned int height) : _mWidth(width), _mHeight(height), Camera(CameraType::PERSPECTIVE) {
		auto& ud = _mCameraUseData;
		auto& wd = _mCameraWorldData;

		wd.projection = glm::perspective(
			glm::radians(_mFov),
			_mWidth / float(_mHeight),
			wd.c_near,
			wd.c_far
		);

		// Calculate initial camera orientation
		wd.front = glm::normalize(wd.target - wd.position);
		wd.right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), wd.front));
		wd.up    = glm::cross(wd.front, wd.right);

		// Ensure that the up vector is perpendicular to the direction vector
		if (glm::length(wd.up) < 0.9f) {
			wd.right = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), wd.front));
			wd.up    = glm::cross(wd.front, wd.right);
		}

		updateLookAt();
	}

	float PerspectiveCamera::getFov() const {
		return _mFov;
	}

	float& PerspectiveCamera::getFov() {
		return _mFov;
	}

	unsigned int PerspectiveCamera::getWidth() const {
		return _mWidth;
	}
	unsigned int& PerspectiveCamera::getWidth() {
		return _mWidth;
	}

	unsigned int PerspectiveCamera::getHeight() const {
		return _mHeight;
	}
	unsigned int& PerspectiveCamera::getHeight() {
		return _mHeight;
	}

	void PerspectiveCamera::draw(const Core::Renderer& renderer, const Core::Shader& shader) const {
		updateShader(shader);
	}

	void PerspectiveCamera::drawUIParams() {
		std::string sname = "Main Camera: #" + std::to_string(_mID.getID());

		std::string sposition = "Position##Camera" + std::to_string(_mID.getID());
		std::string starget   = "Target##Camera"   + std::to_string(_mID.getID());
		std::string sfar      = "Far##Camera"      + std::to_string(_mID.getID());
		std::string snear     = "Near##Camera"     + std::to_string(_mID.getID());
		std::string sfov      = "FOV##Camera"      + std::to_string(_mID.getID());
		std::string swidth    = "Width##Camera"    + std::to_string(_mID.getID());
		std::string sheight   = "Height##Camera"   + std::to_string(_mID.getID());

		if (_mCameraUseData.isActive) {
			ImGui::SeparatorText(sname.c_str());
		}

		if(ImGui::DragFloat3(sposition.c_str(), &_mCameraWorldData.position[0], 1))        { _mCameraUseData.hasUpdate = true; }
		if(ImGui::DragFloat3(starget.c_str(),   &_mCameraWorldData.target[0],   1))        { _mCameraUseData.hasUpdate = true; }
		if(ImGui::DragFloat(sfar.c_str(),       &_mCameraWorldData.c_far,  1, 0, FLT_MAX)) { _mCameraUseData.hasUpdate = true; }
		if(ImGui::DragFloat(snear.c_str(),      &_mCameraWorldData.c_near, 1, 0, FLT_MAX)) { _mCameraUseData.hasUpdate = true; }
		if(ImGui::DragFloat(sfov.c_str(),       &_mFov, 1))                                { _mCameraUseData.hasUpdate = true; }

		int width  = _mWidth;
		int height = _mHeight;

		if(ImGui::DragInt(swidth.c_str(),  &width,  1, 0, INT_MAX)) { _mWidth = width; _mCameraUseData.hasUpdate = true; }
		if(ImGui::DragInt(sheight.c_str(), &height, 1, 0, INT_MAX)) { _mHeight = height; _mCameraUseData.hasUpdate = true; }
	}

	void PerspectiveCamera::updateShader(const Core::Shader &shader) const {
		auto& wd = _mCameraWorldData;

		shader.bind();


		shader.setUniformMatrix4fv("uCamera.view", wd.lookAt);
		shader.setUniformMatrix4fv("uCamera.projection", wd.projection);

		shader.setUniform1f("uCamera.FOV", _mFov);
		shader.setUniform1f("uCamera.width", (float)_mWidth);
		shader.setUniform1f("uCamera.height", (float)_mHeight);

		shader.setUniform1f("uCamera.near", wd.c_near);
		shader.setUniform1f("uCamera.far", wd.c_far);
	}

	void PerspectiveCamera::updateTarget(const Mouse* mouse, float deltaTime) {
		auto& ud = _mCameraUseData;
		auto& wd = _mCameraWorldData;

		// Calculate the mouse movement angles
		float horizontalDelta = deltaTime * ud.mouseSpeed * (_mWidth / 2.0f - mouse->x);
		float verticalDelta = deltaTime * ud.mouseSpeed * (_mHeight / 2.0f - mouse->y);

		// Update the vertical & horizontal angle
		wd.verticalAngle += verticalDelta;
		wd.horizontalAngle += horizontalDelta;

		// Clamp the vertical angle
		wd.verticalAngle = glm::clamp(wd.verticalAngle, -wd.maxUpAngle, wd.maxUpAngle);

		// Calculate the new front direction
		glm::vec3 front(
			cos(wd.verticalAngle) * sin(wd.horizontalAngle),
			sin(wd.verticalAngle),
			cos(wd.verticalAngle) * cos(wd.horizontalAngle)
		);

		// Calculate the new right direction
		glm::vec3 right(
			sin(wd.horizontalAngle - glm::pi<float>() / 2.0f),
			0,
			cos(wd.horizontalAngle - glm::pi<float>() / 2.0f)
		);

		// Calculate the new up direction
		glm::vec3 up = glm::cross(right, front);

		// TODO: Think of better way

		// Update camera properties
		// front * 2.0f so when you try to move you dont start spining
		wd.target = wd.position + (front * 2.0f);
	}

	void PerspectiveCamera::updateProjection() {
		auto& wd = _mCameraWorldData;

		wd.projection = glm::perspective(
			glm::radians(_mFov),
			_mWidth / float(_mHeight),
			wd.c_near,
			wd.c_far
		);
	}

	void PerspectiveCamera::zoom(const Mouse* mouse, float deltaTime) {
		auto& ud = _mCameraUseData;
		if(mouse->hasUpdate) {
			_mFov -= deltaTime * ud.zoomSpeed * mouse->scrollY;
		}

		if (_mFov < 1.0f) {
			_mFov = 10.0f;
		}
		else if(_mFov > 120.0f) {
			_mFov = 120.0f;
		}
	}
};
