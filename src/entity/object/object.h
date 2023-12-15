#pragma once

#include <vector>
#include <memory>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "entity.h"

namespace Engine {
	class Material;

	class Entity;
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

	class Object : public Entity {
		public:
			Object() = delete;
			~Object() = default;

			Object(ObjectType type);

			ObjectType getObjectTpye() const;

			const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;
			std::vector<std::shared_ptr<Mesh>>& getMeshes();

			virtual void load_mesh() = 0;

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			void draw(const Core::Renderer &renderer, const Core::Shader &shader) const override;

			void updateShader(const Core::Shader &shader) const override;

			void UIWorld() override;

		protected:
			void updateModel();

		protected:
			ObjectType _mObjectType;

			std::shared_ptr<Material> _mMaterial;

			std::vector<std::shared_ptr<Mesh>> _mMeshes;
	};
}
