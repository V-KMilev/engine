#pragma once

#include <iostream>

#include "scene.h"

namespace Engine {
	class SceneManager {
		public:
			SceneManager() = delete;
			~SceneManager() = default;

			SceneManager(Scene& scene);

			SceneManager(const SceneManager& other) = delete;
			SceneManager& operator = (const SceneManager& other) = delete;

			SceneManager(SceneManager && other) = delete;
			SceneManager& operator = (SceneManager && other) = delete;

			bool save(const std::string& filePath);
			bool load(const std::string& filePath);

		private:
			bool save_scene(std::ofstream& file);
			bool load_scene(std::ifstream& file);

		private:
			Scene& _mScene;
	};
};
