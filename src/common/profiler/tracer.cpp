#include "tracer.h"

#include <filesystem>

#include "logger.h"

namespace fs = std::filesystem;

namespace Profiler {
	Tracer::Tracer(
		const std::string& fileName,
		const std::string& processName,
		const std::string& threadName
	) : _mFileName(fileName),
		_mProcessName(processName),
		_mThreadName(threadName) {

		_mFolder = "traces";

		if (!fs::exists(_mFolder) && !fs::create_directories(_mFolder)) {
			printf("[ERROR] Failed to create directory: %s\n", _mFolder.c_str());
			exit(-1);
		}

#ifdef MTR_ENABLED
		Engine::LOG("Starting with Profiler...", Engine::LOG_LEVEL::L_INFO);
#endif

		std::string filePath = _mFolder + "/" + _mFileName;

		mtr_init(filePath.c_str());

		MTR_META_PROCESS_NAME(_mProcessName.c_str());
		MTR_META_THREAD_NAME(_mThreadName.c_str());
	}

	Tracer::~Tracer() {
		mtr_flush();
		mtr_shutdown();
	}
}
