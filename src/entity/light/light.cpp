#include "light.h"

#include <string>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_shader.h"

#include "tracer.h"

#include "emit.h"

#include "sphere.h"

namespace Engine {
	Light::Light() : Entity(EntityType::LIGHT) {
		auto emit = addComponent<Emit>();

		_mVisual = std::make_shared<Sphere>();

		auto visualTransform = _mVisual->getComponent<Transform>();

		visualTransform->setPosition(emit->getPosition());
		visualTransform->setScale(glm::vec3(0.25f, 0.25f, 0.25f));

		_mHasUpdate = true;
	}

	Light::Light(LightType type) : _mLightType(type), Entity(EntityType::LIGHT) {
		auto emit = addComponent<Emit>();

		_mVisual = std::make_shared<Sphere>();

		auto visualTransform = _mVisual->getComponent<Transform>();

		visualTransform->setPosition(emit->getPosition());
		visualTransform->setScale(glm::vec3(0.25f, 0.25f, 0.25f));

		_mHasUpdate = true;
	}

	void Light::onUpdate() {
		for(const auto& component : _mComponents) {
			if (component.second->hasUpdate()) {
				_mHasUpdate = true;
			}

			component.second->onUpdate();
		}

		if (_mHasUpdate) {
			auto visualTransform = _mVisual->getComponent<Transform>();
			auto emit = getComponent<Emit>();

			visualTransform->setPosition(emit->getPosition());

			_mVisual->onUpdate();

			// Reset
			_mHasUpdate = false;
		}
	}

	void Light::draw(const Core::Shader &shader) const {
		shader.bind();
	
		_mVisual->draw(shader);
	}

	void Light::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Light", "Light Update Shader");

		auto emit = getComponent<Emit>();

		shader.bind();

		shader.setUniform3fv("uLight.position", emit->getPosition());
		shader.setUniform3fv("uLight.color", emit->getColor());

		PROFILER_END("Light", "Light Update Shader");
	}
};
