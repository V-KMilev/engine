#include "material.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_shader.h"
#include "gl_texture.h"

#include "utils.h"

namespace Engine {

	static std::string TextureTypeToString(TextureType type) {
		switch (type) {
			case TextureType::AMBIENT:            return "Ambient";
			case TextureType::DIFFUSE:            return "Diffuse";
			case TextureType::SPECULAR:           return "Specular";
			case TextureType::SPECULAR_HIGHLIGHT: return "Specular_highlight";
			case TextureType::BUMP:               return "Bump";
			case TextureType::DISPLACEMENT:       return "Displacement";
			case TextureType::ALPHA:              return "Alpha";
			case TextureType::REFLECTION:         return "Reflection";
			case TextureType::ROUGHNESS:          return "Roughness";
			case TextureType::METALLIC:           return "Metallic";
			case TextureType::SHEEN:              return "Sheen";
			case TextureType::EMISSIVE:           return "Emissive";
			case TextureType::NORMAL:             return "Normal";
			default:                              return "NONE";
		}
	}

	Material::Material() {}

	const Textures& Material::getTextures() const {
		return _mTextures;
	}

	Textures& Material::getTextures() {
		return _mTextures;
	}

	const Coefficients& Material::getCoefficients() const{
		return _mCoefficients;
	}

	Coefficients& Material::getCoefficients() {
		return _mCoefficients;
	}

	void Material::updateShader(const Core::Shader &shader) const {
		shader.bind();

		for (int idx = 0; idx < _mTextures.textures.size(); idx++) {

			_mTextures.textures[idx]->bind(idx);

			std::string material = "uMaterial." + TextureTypeToString(TextureType(idx));

			shader.setUniform1i(material, idx);
		}
	}

	void Material::drawUITextures(unsigned int id) {
		std::string sMesh = "Mesh: #" + std::to_string(id);

		if (ImGui::TreeNode(sMesh.c_str())) {
			static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersH | ImGuiTableFlags_RowBg;

			ImGui::Unindent();

			if (ImGui::BeginTable("table1", 3, tableFlags)) {
				ImGui::TableSetupColumn("Texture Tpye:");
				ImGui::TableSetupColumn("Texture:");
				ImGui::TableSetupColumn("Data:");

				ImGui::TableHeadersRow();

				for (int idx = 0; idx < _mTextures.textures.size(); idx++) {
					const std::shared_ptr<Core::Texture>& texture = _mTextures.textures[idx];
					std::string type = TextureTypeToString(TextureType(idx));

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
			ImGui::TreePop();
		}
	}

	bool Material::drawUICoefficients(unsigned int id) {
		bool hasUpdate = false;

		std::string sMesh = "Mesh: #" + std::to_string(id);

		if (ImGui::TreeNode(sMesh.c_str())) {
			static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersH | ImGuiTableFlags_RowBg;

			std::string sAmbient       = "Ambient##Object"       + std::to_string(id);
			std::string sDiffuse       = "Diffuse##Object"       + std::to_string(id);
			std::string sSpecular      = "Specular##Object"      + std::to_string(id);
			std::string sTransmittance = "Transmittance##Object" + std::to_string(id);
			std::string sEmission      = "Emission##Object"      + std::to_string(id);

			std::string sShininess = "Shininess##Object" + std::to_string(id);
			std::string sIOR       = "Ior##Object"       + std::to_string(id);

			std::string sRoughness = "Roughness##Object" + std::to_string(id);
			std::string sMetallic  = "Metallic##Object"  + std::to_string(id);
			std::string sSheen     = "Sheen##Object"     + std::to_string(id);

			ImGui::SeparatorText("Default");

			if (ImGui::ColorEdit3(sAmbient.c_str(),       &_mCoefficients.ambient[0]))       { hasUpdate = true; }
			if (ImGui::ColorEdit3(sDiffuse.c_str(),       &_mCoefficients.diffuse[0]))       { hasUpdate = true; }
			if (ImGui::ColorEdit3(sSpecular.c_str(),      &_mCoefficients.specular[0]))      { hasUpdate = true; }
			if (ImGui::ColorEdit3(sTransmittance.c_str(), &_mCoefficients.transmittance[0])) { hasUpdate = true; }
			if (ImGui::ColorEdit3(sEmission.c_str(),      &_mCoefficients.emission[0]))      { hasUpdate = true; }
			if (ImGui::DragFloat(sShininess.c_str(),      &_mCoefficients.shininess))        { hasUpdate = true; }
			if (ImGui::DragFloat(sIOR.c_str(),            &_mCoefficients.ior, 1))           { hasUpdate = true; }

			ImGui::SeparatorText("PBR");

			if (ImGui::DragFloat(sRoughness.c_str(), &_mCoefficients.roughness, 1)) { hasUpdate = true; }
			if (ImGui::DragFloat(sMetallic.c_str(),  &_mCoefficients.metallic,  1)) { hasUpdate = true; }
			if (ImGui::DragFloat(sSheen.c_str(),     &_mCoefficients.sheen,     1)) { hasUpdate = true; }

			ImGui::TreePop();
		}

		return hasUpdate;
	}
};
