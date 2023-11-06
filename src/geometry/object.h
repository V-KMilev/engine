#pragma once

#include <memory>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"

#include "gl_render.h"
#include "gl_shader.h"
#include "gl_texture.h"

#include "object_id.h"

namespace Engine {
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class VertexBufferLayout;
}

// To avoid including the full glad implementation, we redefine GL_TRIANGLES
#ifndef GL_TRIANGLES
	#define GL_TRIANGLES 0x0004
#endif

namespace Engine {
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

	struct ObjectWorldData {
		public:
			void updateModel();

		public:
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			// roll, pitch, yaw
			glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

			glm::mat4 model = glm::mat4(1.0f);
	};

	class Object {
		public:
			Object();
			virtual ~Object() = default;

			Object(const Object& other);
			Object& operator = (const Object& other);

			Object(Object && other) = delete;
			Object& operator = (Object && other) = delete;

			unsigned int getID() const;
			const Core::Texture& getTexture() const;

			ObjectWorldData& getWorldData();
			const ObjectWorldData& getWorldData() const;

			bool updateTexture(const std::string& file);
			bool updateTexture(const Core::Texture& texture);

			void draw(const Core::Renderer &renderer, const Core::Shader &shader, unsigned int drawType = GL_TRIANGLES) const;

		protected:
			virtual bool init() = 0;
			virtual bool deinit() = 0;

		protected:
			Core::Common::ObjectID _mID;

			ObjectWorldData _mWorldData;

			std::shared_ptr<Core::Texture> _mTexture;

			std::shared_ptr<Core::VertexArray>        _mVA;
			std::shared_ptr<Core::VertexBuffer>       _mVB;
			std::shared_ptr<Core::IndexBuffer>        _mIB;
			std::shared_ptr<Core::VertexBufferLayout> _mVBL;
	};
}
