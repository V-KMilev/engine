#pragma noce

#include <vector>
#include <memory>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "entity.h"

#include "utils.h"

namespace Core {
	class Texture;

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;
}

namespace Engine {
	class Material;
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
	class Mesh : public Entity {
		public:
			Mesh() = delete;
			~Mesh() = default;

			Mesh(
				const std::vector<Utils::Vertex>& vertices,
				const std::vector<unsigned int>& indices,
				const std::shared_ptr<Material>& material
			);

			Mesh(
				const std::vector<Utils::Vertex>& vertices,
				const std::vector<unsigned int>& indices
			);

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			void draw(const Core::Shader &shader) const override;

			void updateShader(const Core::Shader &shader) const override;

		private:
			void init(const std::shared_ptr<Material>& material);

		private:
			std::shared_ptr<Core::VertexArray>        _mVA;
			std::shared_ptr<Core::VertexBuffer>       _mVB;
			std::shared_ptr<Core::IndexBuffer>        _mIB;
			std::shared_ptr<Core::VertexBufferLayout> _mVBL;

		private:
			std::vector<Utils::Vertex>* _mVertices;
			std::vector<unsigned int>* _mIndices;
	};
};
