#pragma once

#include <memory>
#include <map>
#include <string>

#include "object.h"

#include "utils.h"

namespace tinyobj {
	struct attrib_t;
	struct shape_t;
	struct material_t;
}

namespace Core {
	class Texture;
}

namespace Engine {
	class Model : public Object {
		public:
			Model(const std::string& path);

			const std::string& getPath() const;
			std::string& getPath();

			void load_mesh() override;

		private:
			bool load_data();

			void data_fill(
				const tinyobj::attrib_t* attributes,
				const std::vector<tinyobj::shape_t>* shapes,
				const std::vector<tinyobj::material_t>* materials
			);

			std::shared_ptr<Core::Texture> && material_fill(const std::vector<tinyobj::material_t>* materials, int materialID);

			void push_mesh(
				const std::vector<Utils::Vertex>& vertices,
				const std::vector<unsigned int>& indices,
				std::vector<std::shared_ptr<Core::Texture>> && textures
			);

		private:
			std::string _mPath;
	};
};
