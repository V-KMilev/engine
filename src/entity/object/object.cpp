#include "object.h"

#include "tracer.h"

#include "gl_render.h"
#include "gl_shader.h"

#include "gl_error_handle.h"

#include "material.h"
#include "mesh.h"

namespace Engine {
	Object::Object(ObjectType type) : _mObjectType(type), Entity(EntityType::OBJECT) {
		_mMaterial = std::make_shared<Material>();

		_mWorldData = std::make_shared<ObjectWorldData>();
		_mUseData   = std::make_shared<ObjectUseData>();

		// Initial setup
		_mUseData->hasUpdate = true;
	}

	ObjectType Object::getObjectTpye() const {
		return _mObjectType;
	}

	void Object::onUpdate(const Mouse* mouse, float deltaTime) {
		if (_mUseData->hasUpdate) {
			PROFILER_BEGIN("Object", "Object Update");

			updateModel();

			// TODO: Think of a solution to this update prblem (Idea: to scene update)
			for (std::shared_ptr<Mesh>& mesh : _mMeshes) {
				auto worldData = static_cast<ObjectWorldData*>(_mWorldData.get());

				mesh->updateModel(worldData->model);
				mesh->onUpdate(mouse, deltaTime);
			}

			// Reset the update event
			_mUseData->hasUpdate = false;

			PROFILER_END("Object", "Object Update");
		}
	}

	void Object::draw(const Core::Renderer &renderer, const Core::Shader &shader) const {
		PROFILER_BEGIN("Object", "Object Draw");

		updateShader(shader);

		auto useData = static_cast<ObjectUseData*>(_mUseData.get());

		if (useData->linesOnly) {
			MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		}

		for (const std::shared_ptr<Mesh>& mesh : _mMeshes) {
			mesh->draw(renderer, shader);
		}

		MY_GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

		PROFILER_END("Object", "Object Draw");
	}

	void Object::updateShader(const Core::Shader &shader) const {
		PROFILER_BEGIN("Object", "Object Shader Update");

		shader.bind();

		shader.setUniform1ui("uObjectID", _mID.getID());
		shader.setUniform1i("uSelected", _mUseData->isSelected);

		PROFILER_END("Object", "Object Shader Update");
	}

	void Object::updateModel() {
		PROFILER_BEGIN("Object", "Object Update Model");

		auto worldData = static_cast<ObjectWorldData*>(_mWorldData.get());

		worldData->model = glm::mat4(1.0f);

		worldData->model = glm::translate(worldData->model, worldData->position);

		worldData->model = glm::rotate(worldData->model, glm::radians(worldData->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		worldData->model = glm::rotate(worldData->model, glm::radians(worldData->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		worldData->model = glm::rotate(worldData->model, glm::radians(worldData->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		worldData->model = glm::scale(worldData->model, worldData->scale);

		PROFILER_END("Object", "Object Update Model");
	}

	void Object::UIWorld() {
		std::string sObject = "Object: #" + std::to_string(_mID.getID());

		ImGui::SeparatorText(sObject.c_str());

		if (_mUseData->drawUI(_mID.getID())) { _mUseData->hasUpdate = true; }

		ImGui::SeparatorText("World Data");

		if (_mWorldData->drawUI(_mID.getID())) { _mUseData->hasUpdate = true; }
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
