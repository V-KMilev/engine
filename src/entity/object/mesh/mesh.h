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
	struct MeshWorldData : WorldData {
		public:
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			// roll, pitch, yaw
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

			glm::mat4 model = glm::mat4(1.0f);
	};

	struct MeshUseData : UseData {
		public:
	};

	class Mesh : public Entity {
		public:
			Mesh(
				const std::vector<Utils::Vertex>& mVertices,
				const std::vector<unsigned int>&  mIndices,
				const std::vector<unsigned int>&  mTextureIDs
			);

			unsigned int getID() const;

			const MeshWorldData& getWorldData() const override;
			MeshWorldData& getWorldData() override;

			const MeshUseData& getUseData() const override;
			MeshUseData& getUseData() override;

			void onUpdate(const Mouse* mouse, float deltaTime) override;

			void draw(const Core::Renderer &renderer, const Core::Shader &shader) const override;

			void drawUIParams() override;

			void updateModel(glm::mat4 objectModel = glm::mat4(1.0f));

		private:
			void updateShader(const Core::Shader &shader) const override;

		private:
			MeshWorldData _mMeshWorldData;
			MeshUseData   _mMeshUseData;

			std::vector<std::shared_ptr<Core::Texture>> _mTexture;

			std::shared_ptr<Core::VertexArray>        _mVA;
			std::shared_ptr<Core::VertexBuffer>       _mVB;
			std::shared_ptr<Core::IndexBuffer>        _mIB;
			std::shared_ptr<Core::VertexBufferLayout> _mVBL;
	};
};
