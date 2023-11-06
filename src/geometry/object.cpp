#include "object.h"

#include "gl_vertex_array.h"
#include "gl_vertex_buffer.h"
#include "gl_index_buffer.h"
#include "gl_vertex_buffer_layout.h"

namespace Engine {
	void ObjectWorldData::updateModel() {
		if (hasUpdate) {
			model = glm::mat4(1.0f);

			model = glm::translate(model, position);

			model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::scale(model, scale);
		}
	}

	Object::Object() :
		_mTexture(nullptr),
		_mVA(nullptr),
		_mVB(nullptr),
		_mIB(nullptr),
		_mVBL(nullptr)
	{
		// Default texture
#ifdef _WIN32
		const char* defaultTexture = "..\\asset\\textures\\default\\tmp.jpg";
#else
		const char* defaultTexture = "../asset/textures/default/tmp.jpg";
#endif

		_mTexture = std::make_shared<Core::Texture>(defaultTexture);
		_mTexture->init();

		_mWorldData.updateModel();
	}

	Object::Object(const Object& other) {
		_mID = other._mID;

		_mWorldData = other._mWorldData;

		_mTexture = other._mTexture;

		_mVA  = other._mVA;
		_mVB  = other._mVB;
		_mIB  = other._mIB;
		_mVBL = other._mVBL;
	}

	Object& Object::operator=(const Object& other) {
		if (this == &other) {
			return *this;
		}

		_mID = other._mID;

		_mWorldData = other._mWorldData;

		_mTexture = other._mTexture;

		_mVA  = other._mVA;
		_mVB  = other._mVB;
		_mIB  = other._mIB;
		_mVBL = other._mVBL;
		return *this;
	}

	unsigned int Object::getID() const {
		return _mID.getID();
	}

	const Core::Texture& Object::getTexture() const {
		return *_mTexture;
	}

	const ObjectWorldData& Object::getWorldData() const {
		return _mWorldData;
	}

	ObjectWorldData& Object::getWorldData() {
		return _mWorldData;
	}

	bool Object::updateTexture(const std::string& file) {
		_mTexture = nullptr;
		_mTexture = std::make_shared<Core::Texture>(file);

		return _mTexture->update();
	}

	bool Object::updateTexture(const Core::Texture& texture) {
		_mTexture = nullptr;
		_mTexture = std::make_shared<Core::Texture>(texture);

		return _mTexture->update();
	}

	void Object::update() {
		_mWorldData.updateModel();
		_mWorldData.hasUpdate = false;
	}

	void Object::draw(const Core::Renderer& renderer, const Core::Shader& shader, unsigned int drawType) const {
		shader.bind();

		_mTexture->bind(_mTexture->getID());

		shader.setUniformMatrix4fv("uModel", _mWorldData.model);

		std::string texName = _mTexture->getName();
		shader.setUniform1i(texName, _mTexture->getID());

		renderer.draw(*_mVA, *_mIB, shader, drawType);
	}
};
