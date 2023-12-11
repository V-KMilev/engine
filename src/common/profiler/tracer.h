#pragma once

#include <string>

#include "minitrace.h"

#define PROFILER_START(category, name, id) MTR_START(category, name, &id)
#define PROFILER_FINISH(category, name, id) MTR_FINISH(category, name, &id)

#define PROFILER_STEP(category, name, id, step) MTR_STEP(category, name, &id, step);

#define PROFILER_BEGIN(category, name) MTR_BEGIN(category, name)
#define PROFILER_END(category, name) MTR_END(category, name)

#define PROFILER_INSTANT(category, name) MTR_INSTANT(category, name)

namespace Profiler {
	class Tracer {
		public:
			Tracer(
				const std::string& fileName,
				const std::string& processName,
				const std::string& threadName
			);
			~Tracer();

			Tracer(const Tracer& other) = delete;
			Tracer& operator = (const Tracer& other) = delete;

			Tracer(Tracer && other) = delete;
			Tracer& operator = (Tracer && other) = delete;

		private:
			std::string _mFileName;
			std::string _mProcessName;
			std::string _mThreadName;

			std::string _mFolder;
	};
}
