#include "component.h"

#include <string>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "gl_texture.h"

#include "utils.h"

namespace Engine {

// TODO: Add all members as UI part

	bool ObjectWorldData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string sPosition = "Position##Object" + std::to_string(id);
		std::string sRotation = "Rotation##Object" + std::to_string(id);
		std::string sScale    = "Scale##Object"    + std::to_string(id);

		ImGui::SeparatorText("World Data");

		if (ImGui::DragFloat3(sPosition.c_str(), &position[0], _mDragSpeed)) { hasUpdate = true; }
		if (ImGui::DragFloat3(sRotation.c_str(), &rotation[0], _mDragSpeed)) { hasUpdate = true; }
		if (ImGui::DragFloat3(sScale.c_str(),    &scale[0], _mDragSpeed))    { hasUpdate = true; }

		return hasUpdate;
	}

	bool ObjectUseData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string sLinesOnly = "Lines Only##Object" + std::to_string(id);

		if(ImGui::Checkbox(sLinesOnly.c_str(), &linesOnly)) { hasUpdate = true; }

		return hasUpdate;
	}

	bool PerspectiveCameraWorldData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string sPosition = "Position##Camera" + std::to_string(id);
		std::string sTarget   = "Target##Camera"   + std::to_string(id);
		std::string sFar      = "Far##Camera"      + std::to_string(id);
		std::string sNear     = "Near##Camera"     + std::to_string(id);
		std::string sFov      = "FOV##Camera"      + std::to_string(id);
		std::string sWidth    = "Width##Camera"    + std::to_string(id);
		std::string sHeight   = "Height##Camera"   + std::to_string(id);

		if (ImGui::DragFloat3(sPosition.c_str(), &position[0], _mDragSpeed))         { hasUpdate = true; }
		if (ImGui::DragFloat3(sTarget.c_str(),   &target[0], _mDragSpeed))           { hasUpdate = true; }
		if (ImGui::DragFloat(sFar.c_str(),       &c_far, _mDragSpeed, 0, FLT_MAX))   { hasUpdate = true; }
		if (ImGui::DragFloat(sNear.c_str(),      &c_near, _mDragSpeed, 0, FLT_MAX))  { hasUpdate = true; }
		if (ImGui::DragFloat(sFov.c_str(),       &fov, _mDragSpeed, minFOV, maxFOV)) { hasUpdate = true; }

		int tmp_width  = width;
		int tmp_height = height;

		if (ImGui::DragInt(sWidth.c_str(),  &tmp_width, _mDragSpeed, 0, INT_MAX))  { width = tmp_width;   hasUpdate = true; }
		if (ImGui::DragInt(sHeight.c_str(), &tmp_height, _mDragSpeed, 0, INT_MAX)) { height = tmp_height; hasUpdate = true; }

		return hasUpdate;
	}

	bool PerspectiveCameraUseData::drawUI(unsigned int id) {
		bool hasUpdate = false;

		std::string smoveSpeed  = "Move Speed##Camera"  + std::to_string(id);
		std::string smouseSpeed = "Mouse Speed##Camera" + std::to_string(id);
		std::string szoomSpeed  = "Zoom Speed##Camera"  + std::to_string(id);

		if (ImGui::DragFloat(smoveSpeed.c_str(), &moveSpeed, _mDragSpeed, 0, FLT_MAX))   { hasUpdate = true; }
		if (ImGui::DragFloat(smouseSpeed.c_str(), &mouseSpeed, _mDragSpeed, 0, FLT_MAX)) { hasUpdate = true; }
		if (ImGui::DragFloat(szoomSpeed.c_str(), &zoomSpeed, _mDragSpeed, 0, FLT_MAX))   { hasUpdate = true; }

		// TODO: Add debug slot for this
		// UpdateEvent updateEvent = UpdateEvent::NONE;
		// PositionEvent positionEvent = PositionEvent::NONE;

		return hasUpdate;
	}

	bool Textures::drawUI(unsigned int id) {
		bool hasUpdate = false;

		static ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersH | ImGuiTableFlags_RowBg;

		ImGui::Unindent();

		if (ImGui::BeginTable("table1", 3, tableFlags)) {
			ImGui::TableSetupColumn("Texture Tpye:");
			ImGui::TableSetupColumn("Texture:");
			ImGui::TableSetupColumn("Data:");

			ImGui::TableHeadersRow();

			for (int idx = 0; idx < textures.size(); idx++) {
				const std::shared_ptr<Core::Texture>& texture = textures[idx];
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

		return hasUpdate;
	}

	bool Coefficients::drawUI(unsigned int id) {
		bool hasUpdate = false;

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

		if (ImGui::ColorEdit3(sAmbient.c_str(),       &ambient[0]))       { hasUpdate = true; }
		if (ImGui::ColorEdit3(sDiffuse.c_str(),       &diffuse[0]))       { hasUpdate = true; }
		if (ImGui::ColorEdit3(sSpecular.c_str(),      &specular[0]))      { hasUpdate = true; }
		if (ImGui::ColorEdit3(sTransmittance.c_str(), &transmittance[0])) { hasUpdate = true; }
		if (ImGui::ColorEdit3(sEmission.c_str(),      &emission[0]))      { hasUpdate = true; }
		if (ImGui::DragFloat(sShininess.c_str(),      &shininess))        { hasUpdate = true; }
		if (ImGui::DragFloat(sIOR.c_str(),            &ior, 1))           { hasUpdate = true; }

		ImGui::SeparatorText("PBR");

		if (ImGui::DragFloat(sRoughness.c_str(), &roughness, 1)) { hasUpdate = true; }
		if (ImGui::DragFloat(sMetallic.c_str(),  &metallic,  1)) { hasUpdate = true; }
		if (ImGui::DragFloat(sSheen.c_str(),     &sheen,     1)) { hasUpdate = true; }

		return hasUpdate;
	}
};
