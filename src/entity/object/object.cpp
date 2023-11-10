#include "object.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_error_handle.h"

#include "mesh.h"

namespace Engine {
	Object::Object(ObjectType type) : _mObjectType(type), Entity(EntityType::OBJECT) {
		updateModel();
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

			ud.hasUpdate = false;
		}

		for(std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->updateModel(_mObjectWorldData.model);
			mesh->onUpdate(mouse, deltaTime);
		}
	}

	void Object::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		updateShader(shader);

		if (_mObjectUseData.linesOnly) {
			MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		}
		for(const std::shared_ptr<Mesh>& mesh : _mMeshes) {

			mesh->draw(renderer, shader);
		}

		MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	}

	void Object::drawUIParams() {
		auto& ud = _mObjectUseData;
		auto& wd = _mObjectWorldData;

		ImGui::BulletText("Object:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.50f, 0.50f, 0.50f, 1.0f), "%u", _mID.getID());

		std::string position = "Position##Object" + std::to_string(_mID.getID());
		std::string rotation = "Rotation##Object" + std::to_string(_mID.getID());
		std::string scale    = "Scale##Object"    + std::to_string(_mID.getID());
		std::string lines    = "Lines##Object"    + std::to_string(_mID.getID());

		if(ImGui::DragFloat3(position.c_str(), &wd.position[0], 1)) {
			ud.hasUpdate = true;
		}
		if(ImGui::DragFloat3(rotation.c_str(), &wd.rotation[0], 1)) {
			ud.hasUpdate = true;
		}
		if(ImGui::DragFloat3(scale.c_str(), &wd.scale[0], 1)) {
			ud.hasUpdate = true;
		}

		ImGui::Checkbox(lines.c_str(), &_mObjectUseData.linesOnly);

		for(std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->drawUIParams();
		}
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
};
