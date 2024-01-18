#pragma once

#include <fstream>
#include <cstdarg>
#include <vector>
#include <string>

namespace Engine {
	enum class LOG_LEVEL {
		L_INFO  = 0,
		L_WARN  = 1,
		L_ERROR = 2
	};

	class Logger {
		public:
			Logger();
			~Logger();

			Logger(const Logger& other) = delete;
			Logger& operator = (const Logger& other) = delete;

			Logger(Logger && other) = delete;
			Logger& operator = (Logger && other) = delete;

			bool init();
			bool deinit();

			void log(
				const std::string& message,
				LOG_LEVEL level,
				const char* file,
				const int line,
				...
			);

		private:
			std::string leveled_message(
				const std::string& message,
				LOG_LEVEL level,
				const char* file,
				const int line
			) const;

			bool update_file(const std::string& message);

		private:
			std::string _mFileName;
			std::string _mFolder;

			std::ofstream _mFile;

			std::vector<std::string> _mMessages;
	};

	extern Logger logger;

	#define LOG(msg, level, ...)                                 \
		logger.log(msg, level, __FILE__, __LINE__, ##__VA_ARGS__);
};
