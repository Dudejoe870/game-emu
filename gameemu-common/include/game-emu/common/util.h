#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/symbolhelper.h>

namespace GameEmu::Common
{
	namespace Util
	{
		LIBGAMEEMU_COMMON_DLL_EXPORT std::filesystem::path GetExecutablePath();
	}
}
