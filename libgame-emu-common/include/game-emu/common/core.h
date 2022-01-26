#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	class Core
	{
	private:
		static std::vector<Core*> cores;
	public:
		LIBGAMEEMU_COMMON_DLL_EXPORT Core();

		/*
		 Loads all the DLL system cores from the coreSubdirectory (relative to the executable directory)
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT static void InitializeCores(std::filesystem::path coreSubdirectory);
	};
}
