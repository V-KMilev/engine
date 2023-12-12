#include "material.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include "tracer.h"

#include "gl_shader.h"
#include "gl_texture.h"

#include "utils.h"

namespace Engine {
	Material::Material() {
		_mTextures     = std::make_shared<Textures>();
		_mCoefficients = std::make_shared<Coefficients>();
	}

	const std::shared_ptr<Textures>& Material::getTextures() const {
		return _mTextures;
	}

	std::shared_ptr<Textures>& Material::getTextures() {
		return _mTextures;
	}

	const std::shared_ptr<Coefficients>& Material::getCoefficients() const{
		return _mCoefficients;
	}

	std::shared_ptr<Coefficients>& Material::getCoefficients() {
		return _mCoefficients;
	}

	void Material::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Material", "Material Shader Update");

		shader.bind();

		for (int idx = 0; idx < _mTextures->textures.size(); idx++) {
			_mTextures->textures[idx]->bind(idx);

			std::string material = "uMaterial." + TextureTypeToString(TextureType(idx));

			shader.setUniform1i(material, idx);
		}

		PROFILER_END("Material", "Material Shader Update");
	}

	void Material::UITextures(unsigned int id) {
		std::string sMesh = "Mesh: #" + std::to_string(id);

		if (ImGui::TreeNode(sMesh.c_str())) {
			_mTextures->drawUI(id);
			ImGui::TreePop();
		}
	}

	bool Material::UICoefficients(unsigned int id) {
		bool hasUpdate = false;

		std::string sMesh = "Mesh: #" + std::to_string(id);

		if (ImGui::TreeNode(sMesh.c_str())) {
			_mCoefficients->drawUI(id);
			ImGui::TreePop();
		}

		return hasUpdate;
	}
};
