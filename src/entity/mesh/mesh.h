#pragma noce

#include <vector>
#include <memory>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "object.h"

#include "utils.h"

namespace Core {
	class Texture;

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;
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
	class Mesh : public Object {
		public:
			Mesh(
				const std::vector<Utils::Vertex>& vertices,
				const std::vector<unsigned int>& indices,
				std::shared_ptr<Material> material
			);

			void load_mesh() override;

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			void draw(const Core::Shader &shader) const override;

			void updateShader(const Core::Shader &shader) const override;

			void UIWorld() override;
			void UIMaterialTextures();
			void UIMaterialCoefficients();

		public:
			void updateModel(glm::mat4 objectModel = glm::mat4(1.0f));

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
