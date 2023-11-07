#pragma once

#include "object.h"

// To avoid including the full glad implementation, we redefine GL_TRIANGLES
#ifndef GL_TRIANGLES
	#define GL_TRIANGLES 0x0004
#endif

namespace Engine {
	class Quad : public Object {
		public:
			Quad();
			~Quad() = default;

			Quad(const Quad& other);
			Quad& operator = (const Quad& other);

			Quad(Quad && other) = delete;
			Quad& operator = (Quad && other) = delete;

		private:
			const float _mVertices[QuadVerticesSize + TextureVertices * QuadVertices] = {
				// positions        // texture Coords
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,    //bottom left
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,    //bottom right
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,    //top left
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f     //top right
			};

			const unsigned int _mIndices[QuadIndicesSize] = {
				0, 1, 2,
				2, 1, 3
			};
	};
};
