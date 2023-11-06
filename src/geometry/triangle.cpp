#include "triangle.h"

namespace Engine {
	Triangle::Triangle() : Object() {
		init();
	}

	Triangle::Triangle(const Triangle& other) : Object() {}

	Triangle& Triangle::operator = (const Triangle& other) {
		if (this == &other) {
			return *this;
		}

		Object::operator=(other);

		return *this;
	}

	bool Triangle::init() {
		_mVB = std::make_shared<Core::VertexBuffer>(&_mVertices[0], (TriangleVerticesSize + TextureVertices * TriangleVertices) * sizeof(float));
		_mIB = std::make_shared<Core::IndexBuffer>(&_mIndices[0], TriangleIndices);
		_mVA = std::make_shared<Core::VertexArray>();

		// 3 floats for position, 2 floats for texture coordinates
		_mVBL = std::make_shared<Core::VertexBufferLayout>();
		_mVBL->push<float>(3);
		_mVBL->push<float>(2);

		_mVA->addBuffer(*_mVB, *_mVBL);

		return true;
	}

	bool Triangle::deinit() {
		return true;
	}
};
