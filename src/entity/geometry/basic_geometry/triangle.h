#pragma once

#include "object.h"

// To avoid including the full glad implementation, we redefine GL_TRIANGLES
#ifndef GL_TRIANGLES
	#define GL_TRIANGLES 0x0004
#endif

namespace Engine {
	class Triangle : public Object {
		public:
			Triangle();
			~Triangle() = default;

			Triangle(const Triangle& other);
			Triangle& operator = (const Triangle& other);

			Triangle(Triangle && other) = delete;
			Triangle& operator = (Triangle && other) = delete;

		private:
			static constexpr float _mVertices[TriangleVerticesSize + TextureVertices * TriangleVertices] = {
				// positions        // texture Coords
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,    // bottom left
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom right
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f     // top left
			};

			static constexpr unsigned int _mIndices[TriangleIndicesSize] = {
				0, 1, 2
			};
	};
};
