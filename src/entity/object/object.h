#pragma once

#include <vector>
#include <memory>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "entity.h"

namespace Engine {
	class Entity;

	struct WorldData;

	class Mesh;
}

// To avoid including the full glad implementation, we redefine GL_TRIANGLES
#ifndef GL_TRIANGLES
	#define GL_TRIANGLES 0x0004
#endif

// Basic define primitive geometry
#define Dimensions 3

#define TextureVertices  2
#define TriangleVertices 3
#define QuadVertices     4

#define TriangleVerticesSize Dimensions * TriangleVertices
#define QuadVerticesSize     Dimensions * QuadVertices

#define TriangleIndices 3
#define QuadIndices     6

#define TriangleIndicesSize TriangleIndices
#define QuadIndicesSize     QuadIndices

namespace Engine {
	enum class ObjectType {
		NONE      = 0,
		TRIANGLE  = 1,
		QUAD      = 2,
		MODEL     = 3,
		MESH      = 4
	};

	struct ObjectWorldData : WorldData {
		public:
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			// roll, pitch, yaw
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

			glm::mat4 model = glm::mat4(1.0f);
	};

	struct RenderData {
		glm::vec3 ambient       = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuse       = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 specular      = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 transmittance = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 emission      = glm::vec3(0.0f, 0.0f, 0.0f);

		float shininess = 0.0f;
		// index of refraction
		float ior       = 0.0f;

		float roughness = 0.0f;
		float metallic  = 0.0f;
		float sheen     = 0.0f;
	};

	struct ObjectUseData : UseData {
		public:
			bool linesOnly = false;
	};

	class Object : public Entity {
		public:
			Object() = delete;
			~Object() = default;

			Object(ObjectType type);

			ObjectType getObjectTpye() const;

			const ObjectWorldData& getWorldData() const override;
			ObjectWorldData& getWorldData() override;

			const ObjectUseData& getUseData() const override;
			ObjectUseData& getUseData() override;

			const RenderData& getRenderData() const;
			RenderData& getRenderData();

			virtual void load_mesh() = 0;

		public:
			void onUpdate(const Mouse* mouse, float deltaTime) override;

			void draw(const Core::Renderer &renderer, const Core::Shader &shader) const override;

			void drawUIParams() override;
			void drawUIRenderData();
			void drawUIMeshList();
			void drawUIMeshTextures();

		protected:
			void updateShader(const Core::Shader &shader) const override;

			void updateModel();

		protected:
			ObjectType _mObjectType;

			ObjectWorldData _mObjectWorldData;
			ObjectUseData _mObjectUseData;

			RenderData _mRenderData;

			std::vector<std::shared_ptr<Mesh>> _mMeshes;
	};
}
