#include "material.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "gl_shader.h"
#include "gl_texture.h"

#include "utils.h"

namespace Engine {
	Material::Material() : Component(ComponentType::MATERIAL) {
		textures     = std::make_shared<Textures>();
		coefficients = std::make_shared<Coefficients>();

		on_textures     = std::make_shared<Textures>();
		on_coefficients = std::make_shared<Coefficients>();

		// Default texture
		std::string defaultPath = "../asset/textures/default/texture.png";

		for (std::shared_ptr<Core::Texture>& texture : textures->textures) {
			texture = std::make_shared<Core::Texture>(defaultPath);
		}
	}

	const std::vector<std::shared_ptr<Core::Texture>>& Material::getTextures() const { return textures->textures; }
	std::vector<std::shared_ptr<Core::Texture>>& Material::getTextures() { return textures->textures; }

	const glm::vec3& Material::getAmbient() const       { return coefficients->ambient; }
	const glm::vec3& Material::getDiffuse() const       { return coefficients->diffuse; }
	const glm::vec3& Material::getSpecular() const      { return coefficients->specular; }
	const glm::vec3& Material::getTransmittance() const { return coefficients->transmittance; }
	const glm::vec3& Material::getEmission() const      { return coefficients->emission; }

	float Material::getShininess() const { return coefficients->shininess; }
	float Material::getIor() const       { return coefficients->ior; }
	float Material::getRoughness() const { return coefficients->roughness; }
	float Material::getMetallic() const  { return coefficients->metallic; }
	float Material::getSheen() const     { return coefficients->sheen; }

	void Material::setAmbient(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(coefficients->ambient, on_coefficients->ambient, _mHasUpdate, value, updateInstant);
	}

	void Material::setDiffuse(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(coefficients->diffuse, on_coefficients->diffuse, _mHasUpdate, value, updateInstant);
	}

	void Material::setSpecular(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(coefficients->specular, on_coefficients->specular, _mHasUpdate, value, updateInstant);
	}

	void Material::setTransmittance(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(coefficients->transmittance, on_coefficients->transmittance, _mHasUpdate, value, updateInstant);
	}

	void Material::setEmission(const glm::vec3& value, bool updateInstant) {
		setValue<glm::vec3>(coefficients->emission, on_coefficients->emission, _mHasUpdate, value, updateInstant);
	}

	void Material::setShininess(float value, bool updateInstant) {
		setValue<float>(coefficients->shininess, on_coefficients->shininess, _mHasUpdate, value, updateInstant);
	}

	void Material::setIor(float value, bool updateInstant) {
		setValue<float>(coefficients->ior, on_coefficients->ior, _mHasUpdate, value, updateInstant);
	}

	void Material::setRoughness(float value, bool updateInstant) {
		setValue<float>(coefficients->roughness, on_coefficients->roughness, _mHasUpdate, value, updateInstant);
	}

	void Material::setMetallic(float value, bool updateInstant) {
		setValue<float>(coefficients->metallic, on_coefficients->metallic, _mHasUpdate, value, updateInstant);
	}

	void Material::setSheen(float value, bool updateInstant) {
		setValue<float>(coefficients->sheen, on_coefficients->sheen, _mHasUpdate, value, updateInstant);
	}

	json Material::toJson() const {
		json componentJson;

		json texturesArray;
		for (int i = 0; i < textures->textures.size(); ++i) {
			componentJson.push_back(textures->textures[i]->getPath());
		}

		componentJson["textures"] = texturesArray;

		componentJson["ambient"]       = {coefficients->ambient.x, coefficients->ambient.y, coefficients->ambient.z};
		componentJson["diffuse"]       = {coefficients->diffuse.x, coefficients->diffuse.y, coefficients->diffuse.z};
		componentJson["specular"]      = {coefficients->specular.x, coefficients->specular.y, coefficients->specular.z};
		componentJson["transmittance"] = {coefficients->transmittance.x, coefficients->transmittance.y, coefficients->transmittance.z};
		componentJson["emission"]      = {coefficients->emission.x, coefficients->emission.y, coefficients->emission.z};

		componentJson["shininess"] = coefficients->shininess;
		componentJson["ior"]       = coefficients->ior;

		componentJson["roughness"] = coefficients->roughness;
		componentJson["metallic"]  = coefficients->metallic;
		componentJson["sheen"]     = coefficients->sheen;

		return componentJson;
	}

	void Material::onUpdate() {
		if(_mHasUpdate) {
			coefficients = on_coefficients;
			// TODO:
			// textures = on_textures;

			// Reset
			_mHasUpdate = false;
		}
	}

	void Material::drawUI() const {
		bool hasUpdate = false;

		static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersH | ImGuiTableFlags_RowBg;

		if (ImGui::BeginTable("table1", 3, tableFlags)) {
			ImGui::TableSetupColumn("Texture Type:");
			ImGui::TableSetupColumn("Texture:");
			ImGui::TableSetupColumn("Data:");

			ImGui::TableHeadersRow();

			for (int idx = 0; idx < textures->textures.size(); idx++) {
				const std::shared_ptr<Core::Texture>& texture = textures->textures[idx];
				std::string type = getNameFromType(TextureType(idx));

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::NewLine();
				ImGui::NewLine();
				ImGui::Indent();
				ImGui::TextColored(ImVec4(0.96f, 0.26f, 0.26f, 1.00f), "%s", type.c_str());
				ImGui::Unindent();

				ImGui::TableSetColumnIndex(1);
				ImGui::Image((ImTextureID)texture->getID(), ImVec2(100.0f, 100.0f));

				ImGui::TableSetColumnIndex(2);

				Utils::UI::ColoredBulletText("ID:", std::to_string(texture->getID()), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
				Utils::UI::ColoredBulletText("Width:", std::to_string(texture->getWidth()), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
				Utils::UI::ColoredBulletText("Height:", std::to_string(texture->getHeight()), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
				Utils::UI::ColoredBulletText("Path:", texture->getPath(), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
				Utils::UI::ColoredBulletText("Name:", texture->getName(), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));

				if (ImGui::TreeNodeEx("OpenGL Data", ImGuiTreeNodeFlags_Selected)) {
					const auto& tParams = texture->getParams();

					Utils::UI::ColoredBulletText("Border:", std::to_string(tParams.border), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Target:", std::to_string(tParams.target), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Level:", std::to_string(tParams.level), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Internal Format:", std::to_string(tParams.internalFormat), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Border:", std::to_string(tParams.border), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Format:", std::to_string(tParams.format), ImVec4(0.76f, 0.26f, 0.26f, 1.00f));
					Utils::UI::ColoredBulletText("Type:", std::to_string(tParams.type), ImVec4(0.86f, 0.26f, 0.26f, 1.00f));

					ImGui::TreePop();
				}
			}
			ImGui::EndTable();
		}

		std::string sAmbient       = "Ambient##Object"       + std::to_string(_mID);
		std::string sDiffuse       = "Diffuse##Object"       + std::to_string(_mID);
		std::string sSpecular      = "Specular##Object"      + std::to_string(_mID);
		std::string sTransmittance = "Transmittance##Object" + std::to_string(_mID);
		std::string sEmission      = "Emission##Object"      + std::to_string(_mID);

		std::string sShininess = "Shininess##Object" + std::to_string(_mID);
		std::string sIOR       = "Ior##Object"       + std::to_string(_mID);

		std::string sRoughness = "Roughness##Object" + std::to_string(_mID);
		std::string sMetallic  = "Metallic##Object"  + std::to_string(_mID);
		std::string sSheen     = "Sheen##Object"     + std::to_string(_mID);

		ImGui::SeparatorText("Diffuse");

		if (ImGui::ColorEdit3(sAmbient.c_str(),       &on_coefficients->ambient[0]))       { _mHasUpdate = true; }
		if (ImGui::ColorEdit3(sDiffuse.c_str(),       &on_coefficients->diffuse[0]))       { _mHasUpdate = true; }
		if (ImGui::ColorEdit3(sSpecular.c_str(),      &on_coefficients->specular[0]))      { _mHasUpdate = true; }
		if (ImGui::ColorEdit3(sTransmittance.c_str(), &on_coefficients->transmittance[0])) { _mHasUpdate = true; }
		if (ImGui::ColorEdit3(sEmission.c_str(),      &on_coefficients->emission[0]))      { _mHasUpdate = true; }

		ImGui::SeparatorText("PBR");

		if (ImGui::DragFloat(sRoughness.c_str(), &on_coefficients->roughness, _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sMetallic.c_str(),  &on_coefficients->metallic,  _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sSheen.c_str(),     &on_coefficients->sheen,     _mDragSpeed)) { _mHasUpdate = true; }

		ImGui::SeparatorText("Ray-Trace");

		if (ImGui::DragFloat(sShininess.c_str(), &on_coefficients->shininess, _mDragSpeed)) { _mHasUpdate = true; }
		if (ImGui::DragFloat(sIOR.c_str(),       &on_coefficients->ior, _mDragSpeed))       { _mHasUpdate = true; }
	}

	void Material::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Material", "Material Shader Update");

		shader.bind();

		for (int idx = 0; idx < textures->textures.size(); idx++) {
			textures->textures[idx]->bind(idx);

			std::string material = "uMaterial." + getNameFromType(TextureType(idx));

			shader.setUniform1i(material, idx);
		}

		PROFILER_END("Material", "Material Shader Update");
	}
};
