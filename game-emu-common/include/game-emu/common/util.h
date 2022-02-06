#pragma once

#include <string>
#include <filesystem>

#include <game-emu/common/symbolhelper.h>

namespace GameEmu::Common
{
	namespace Util
	{
		LIBGAMEEMU_COMMON_DLL_EXPORT std::filesystem::path getExecutablePath();
	}
}
