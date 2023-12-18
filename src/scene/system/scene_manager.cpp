#include "scene_manager.h"

#include <filesystem>
#include <fstream>

#include "json.hpp"

#include "logger.h"

namespace fs = std::filesystem;

namespace Engine {
	SceneManager::SceneManager(Scene& scene) : _mScene(scene) {}

	bool SceneManager::save(const std::string& filePath) {

		std::string folder = fs::path(filePath).parent_path().string();

		if (!fs::exists(folder) && !fs::create_directories(folder)) {
			LOG("[ERROR] Failed to create directory: %s\n", LOG_LEVEL::L_WARN, folder.c_str());
			return false;
		}

		std::ofstream file = std::ofstream(filePath);

		if (!file.is_open()) {
			std::string fileName = fs::path(filePath).filename().string();

			LOG("[ERROR] Failed to open file: %s\n", LOG_LEVEL::L_WARN, fileName.c_str());
			return false;
		}

		return save_scene(file);
	}

	bool SceneManager::load(const std::string& filePath) {

		std::ifstream file = std::ifstream(filePath);

		if (!file.is_open()) {
			LOG("[ERROR] Failed to open file: %s\n", LOG_LEVEL::L_WARN, filePath.c_str());
			return false;
		}

		return load_scene(file);
	}

	bool SceneManager::save_scene(std::ofstream& file) {

		json jsonEntities;

		auto& entitys = _mScene.getEntitys();

		for(auto& entity : entitys) {
			jsonEntities.push_back(entity->toJson());
		}

		file << std::setw(4) << jsonEntities << std::endl;

		return !file.fail();
	}

	bool SceneManager::load_scene(std::ifstream& file) {



		return true;
	}
};