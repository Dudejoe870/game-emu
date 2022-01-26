#include <game-emu/common/util.h>

#if defined(_WIN32) || defined(__CYGWIN__)
#include <Windows.h>
#elif defined(__linux__)
#include <limits.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h>
#elif
#error "Platform not supported!"
#endif

namespace GameEmu::Common
{
	std::filesystem::path Util::getExecutablePath()
	{
#if defined(_WIN32) || defined(__CYGWIN__)
		wchar_t path[MAX_PATH] = { 0 };
		GetModuleFileNameW(NULL, path, MAX_PATH);
		return path;
#elif defined(__linux__)
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);
#elif defined(__APPLE__)
		char result[PATH_MAX];
		uint32_t count = PATH_MAX;
		_NSGetExecutablePath(result, &count);
		return std::string(result, (count > 0) ? count : 0);
#endif
	}
}
