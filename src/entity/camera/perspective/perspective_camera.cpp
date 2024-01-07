#include "perspective_camera.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "gl_shader.h"

#include "input_manager.h"

#include "view.h"
#include "object.h"

namespace Engine {
	PerspectiveCamera::PerspectiveCamera(unsigned int width, unsigned int height) : Camera(CameraType::PERSPECTIVE) {
		auto view = getComponent<PerspectiveView>();

		view->calculateInitView(width, height);
	}

	void PerspectiveCamera::drawUI() const {
		std::string sCamera = "Camera: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sCamera.c_str());

		Entity::drawUI();

		for(const auto& component : _mComponents) {
			component.second->drawUI();
		}
	}

	void PerspectiveCamera::draw(const Core::Shader& shader) const {
		shader.bind();

		_mVisual->draw(shader);
	}

	void PerspectiveCamera::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Camera", "Camera Update Shader");

		shader.bind();

		auto view = getComponent<PerspectiveView>();

		shader.setUniformMatrix4fv("uCamera.view", view->getLookAt());
		shader.setUniformMatrix4fv("uCamera.projection", view->getProjection());

		shader.setUniform1f("uCamera.FOV", view->getFov());
		shader.setUniform1f("uCamera.width", (float)view->getWidth());
		shader.setUniform1f("uCamera.height", (float)view->getHeight());

		shader.setUniform1f("uCamera.near", view->getNear());
		shader.setUniform1f("uCamera.far", view->getFar());

		PROFILER_END("Camera", "Camera Update Shader");
	}

	void PerspectiveCamera::updateTarget(const Mouse* mouse, float deltaTime) {
		auto view = getComponent<PerspectiveView>();

		view->updateTarget(mouse, deltaTime);
	}

	void PerspectiveCamera::zoom(const Mouse* mouse, float deltaTime) {
		auto view = getComponent<PerspectiveView>();

		view->zoom(mouse, deltaTime);
	}
};