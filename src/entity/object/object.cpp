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

	const RenderData& Object::getRenderData() const {
		return _mRenderData;
	}

	RenderData& Object::getRenderData() {
		return _mRenderData;
	}

	void Object::onUpdate(const Mouse* mouse, float deltaTime) {
		auto& ud = _mObjectUseData;

		if (ud.hasUpdate) {
			updateModel();

			// Reset the update event
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

	void Object::drawUIParams() {
		auto& ud = _mObjectUseData;
		auto& wd = _mObjectWorldData;
		auto& rd = _mRenderData;

		std::string sObject    = "Object: #" + std::to_string(_mID.getID());
		std::string sLinesOnly = "Lines Only##Object" + std::to_string(_mID.getID());

		std::string sPosition = "Position##Object" + std::to_string(_mID.getID());
		std::string sRotation = "Rotation##Object" + std::to_string(_mID.getID());
		std::string sScale    = "Scale##Object"    + std::to_string(_mID.getID());

		ImGui::SeparatorText(sObject.c_str());

		ImGui::Checkbox(sLinesOnly.c_str(), &_mObjectUseData.linesOnly);

		ImGui::SeparatorText("World Data");

		if(ImGui::DragFloat3(sPosition.c_str(), &wd.position[0], 1)) { ud.hasUpdate = true; }
		if(ImGui::DragFloat3(sRotation.c_str(), &wd.rotation[0], 1)) { ud.hasUpdate = true; }
		if(ImGui::DragFloat3(sScale.c_str(),    &wd.scale[0], 1))    { ud.hasUpdate = true; }
	}

	void Object::drawUIRenderData() {
		auto& ud = _mObjectUseData;
		auto& rd = _mRenderData;

		std::string sAmbient       = "Ambient##Object"       + std::to_string(_mID.getID());
		std::string sDiffuse       = "Diffuse##Object"       + std::to_string(_mID.getID());
		std::string sSpecular      = "Specular##Object"      + std::to_string(_mID.getID());
		std::string sTransmittance = "Transmittance##Object" + std::to_string(_mID.getID());
		std::string sEmission      = "Emission##Object"      + std::to_string(_mID.getID());

		std::string sShininess = "Shininess##Object" + std::to_string(_mID.getID());
		std::string sIOR       = "Ior##Object"       + std::to_string(_mID.getID());

		std::string sRoughness = "Roughness##Object" + std::to_string(_mID.getID());
		std::string sMetallic  = "Metallic##Object"  + std::to_string(_mID.getID());
		std::string sSheen     = "Sheen##Object"     + std::to_string(_mID.getID());

		ImGui::SeparatorText("Default");

		if(ImGui::ColorEdit3(sAmbient.c_str(),       &rd.ambient[0]))       { ud.hasUpdate = true; }
		if(ImGui::ColorEdit3(sDiffuse.c_str(),       &rd.diffuse[0]))       { ud.hasUpdate = true; }
		if(ImGui::ColorEdit3(sSpecular.c_str(),      &rd.specular[0]))      { ud.hasUpdate = true; }
		if(ImGui::ColorEdit3(sTransmittance.c_str(), &rd.transmittance[0])) { ud.hasUpdate = true; }
		if(ImGui::ColorEdit3(sEmission.c_str(),      &rd.emission[0]))      { ud.hasUpdate = true; }
		if(ImGui::DragFloat(sShininess.c_str(),      &rd.shininess))        { ud.hasUpdate = true; }
		if(ImGui::DragFloat(sIOR.c_str(),            &rd.ior, 1))           { ud.hasUpdate = true; }

		ImGui::SeparatorText("PBR");

		if(ImGui::DragFloat(sRoughness.c_str(), &rd.roughness, 1)) { ud.hasUpdate = true; }
		if(ImGui::DragFloat(sMetallic.c_str(),  &rd.metallic,  1)) { ud.hasUpdate = true; }
		if(ImGui::DragFloat(sSheen.c_str(),     &rd.sheen,     1)) { ud.hasUpdate = true; }
	}

	void Object::drawUIMeshList() {
		std::string sMeshList  = "Mesh List##Object" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sMeshList.c_str());

		for(std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->drawUIParams();
		}
	}

	void Object::drawUIMeshTextures() {
		std::string sMeshTextures  = "Mesh Texture List##Object" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sMeshTextures.c_str());

		for(std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->drawUiTextures();
		}
	}
};
