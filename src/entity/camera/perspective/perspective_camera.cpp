#include "perspective_camera.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "gl_shader.h"

#include "view.h"

#include "object.h"

namespace Engine {
	PerspectiveCamera::PerspectiveCamera(unsigned int width, unsigned int height) : Camera(CameraType::PERSPECTIVE) {
		auto view = getComponent<PerspectiveView>();

		view->calculateInitView(width, height);
	}

	void PerspectiveCamera::draw(const Core::Shader& shader) const {
		shader.bind();

		_mVisual->draw(shader);
	}

	void PerspectiveCamera::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Camera", "Camera Update Shader");

		auto view = getComponent<PerspectiveView>();

		static std::string sView       = "uCamera.view";
		static std::string sProjection = "uCamera.projection";
		static std::string sPosition   = "uCamera.position";
		static std::string sFOV        = "uCamera.FOV";
		static std::string sWidth      = "uCamera.width";
		static std::string sHeight     = "uCamera.height";
		static std::string sNear       = "uCamera.near";
		static std::string sFar        = "uCamera.far";

		shader.bind();

		shader.setUniformMatrix4fv(sView, view->getLookAt());
		shader.setUniformMatrix4fv(sProjection, view->getProjection());

		shader.setUniform3fv(sPosition, view->getPosition());

		shader.setUniform1f(sFOV, view->getFov());
		shader.setUniform1f(sWidth, (float)view->getWidth());
		shader.setUniform1f(sHeight, (float)view->getHeight());

		shader.setUniform1f(sNear, view->getNear());
		shader.setUniform1f(sFar, view->getFar());

		PROFILER_END("Camera", "Camera Update Shader");
	}

	void PerspectiveCamera::updateTarget() {
		auto view = getComponent<PerspectiveView>();

		view->updateTarget();
	}

	void PerspectiveCamera::zoom() {
		auto view = getComponent<PerspectiveView>();

		view->zoom();
	}
};
