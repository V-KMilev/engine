#include "logger.h"

#include <filesystem>

#include "utils.h"

namespace fs = std::filesystem;

namespace Engine {
	Logger::Logger() : _folder(""), _fileName(""), _file(), _messages({}) {
		init();
	}

	Logger::~Logger() {
		deinit();
	}

	bool Logger::init() {
		const std::string time = Engine::Utils::get_current_time("%Y-%m-%d_%H-%M-%S");

		_folder = "log";

		if (!fs::exists(_folder) && !fs::create_directories(_folder)) {
			printf("[ERROR] Failed to create directory: %s\n", _folder.c_str());
			return false;
		}

		_fileName = _folder + "/log-" + time + ".txt";
		_file     = std::ofstream(_fileName);

		if (!_file.is_open()) {
			printf("[ERROR] Failed to open log file: %s\n", _fileName.c_str());
			return false;
		}

		_messages.clear();

		return true;
	}

	bool Logger::deinit() {
		if (_file.is_open()) {
			_file.close();
		}

		return true;
	}

	void Logger::log(
		const std::string& message,
		LOG_LEVEL level,
		const char* file,
		const int line,
		...
	) {
		va_list args;
		va_start(args, line);

		char buffer[256];

		vsnprintf(buffer, sizeof(buffer), message.c_str(), args);
		va_end(args);

		std::string out_message = leveled_message(
			buffer,
			level,
			file,
			line
		);

		printf("%s", out_message.c_str());

		if (!update_file(out_message)) {
			printf("[ERROR] Failed to update current log file\n");
		}

		_messages.push_back(out_message);
	}

	std::string Logger::leveled_message(
		const std::string& message,
		LOG_LEVEL level,
		const char* file,
		const int line
	) const {
		const std::string time = Engine::Utils::get_current_time();

		std::string leveled_message = time + "\t";

		if (level == LOG_LEVEL::L_INFO) {
			leveled_message += "[INFO] ";
		}
		else if (level == LOG_LEVEL::L_WARN) {
			leveled_message += "[WARN] ";
		}
		else if (level == LOG_LEVEL::L_ERROR) {
			leveled_message += "[ERROR][" + std::string(file) + ":" + std::to_string(line) + "]";
		}

		leveled_message += " " + message + "\n";

		return leveled_message;
	}

	bool Logger::update_file(const std::string& message) {
		if (_file) {
			_file << message;

			return true;
		}
		return false;
	}

	Logger logger;
}
