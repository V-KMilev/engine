#include "object.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_error_handle.h"

#include "material.h"
#include "mesh.h"

namespace Engine {
	Object::Object(ObjectType type) : _mObjectType(type), Entity(EntityType::OBJECT) {
		_mMaterial = std::make_shared<Material>();
		_mObjectUseData.hasUpdate = true;
	}

	ObjectType Object::getObjectTpye() const {
		return _mObjectType;
	}

	const ObjectWorldData& Object::getWorldData() const {
		return _mObjectWorldData;
	}

	ObjectWorldData& Object::getWorldData() {
		return _mObjectWorldData;
	}

	const ObjectUseData& Object::getUseData() const {
		return _mObjectUseData;
	}

	ObjectUseData& Object::getUseData() {
		return _mObjectUseData;
	}

	void Object::onUpdate(const Mouse* mouse, float deltaTime) {
		auto& ud = _mObjectUseData;

		if (ud.hasUpdate) {
			updateModel();

			// Reset the update event
			ud.hasUpdate = false;
		}

		for (std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->updateModel(_mObjectWorldData.model);
			mesh->onUpdate(mouse, deltaTime);
		}
	}

	void Object::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		updateShader(shader);

		if (_mObjectUseData.linesOnly) {
			MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		}

		for (const std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->draw(renderer, shader);
		}

		MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	void Object::updateShader(const Core::Shader &shader) const {
		shader.bind();

		shader.setUniform1ui("uObjectID", _mID.getID());
		shader.setUniform1i("uSelected", _mObjectUseData.isSelected);
	}

	void Object::updateModel() {
		auto& wd = _mObjectWorldData;

		wd.model = glm::mat4(1.0f);

		wd.model = glm::translate(wd.model, wd.position);

		wd.model = glm::rotate(wd.model, glm::radians(wd.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		wd.model = glm::rotate(wd.model, glm::radians(wd.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		wd.model = glm::rotate(wd.model, glm::radians(wd.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		wd.model = glm::scale(wd.model, wd.scale);
	}

	void Object::UIWorld() {
		auto& ud = _mObjectUseData;
		auto& wd = _mObjectWorldData;

		std::string sObject    = "Object: #" + std::to_string(_mID.getID());
		std::string sLinesOnly = "Lines Only##Object" + std::to_string(_mID.getID());

		std::string sPosition = "Position##Object" + std::to_string(_mID.getID());
		std::string sRotation = "Rotation##Object" + std::to_string(_mID.getID());
		std::string sScale    = "Scale##Object"    + std::to_string(_mID.getID());

		ImGui::SeparatorText(sObject.c_str());

		ImGui::Checkbox(sLinesOnly.c_str(), &_mObjectUseData.linesOnly);

		ImGui::SeparatorText("World Data");

		if (ImGui::DragFloat3(sPosition.c_str(), &wd.position[0], 1)) { ud.hasUpdate = true; }
		if (ImGui::DragFloat3(sRotation.c_str(), &wd.rotation[0], 1)) { ud.hasUpdate = true; }
		if (ImGui::DragFloat3(sScale.c_str(),    &wd.scale[0], 1))    { ud.hasUpdate = true; }
	}

	void Object::UIMeshsWorld() {
		for (std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->UIWorld();
		}
	}

	void Object::UIMeshsMaterialTextures() {
		for (std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->UIMaterialTextures();
		}
	}

	void Object::UIMeshsMaterialCoefficients() {
		for (std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->UIMaterialCoefficients();
		}
	}
};
