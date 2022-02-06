#pragma once

#include <game-emu/common/stdcommon.h>

#include <dynalo/dynalo.hpp>

namespace GameEmu::Common
{
	class Core;

	class CoreLoader
	{
	private:
		std::vector<Core*> cores;
		std::vector<dynalo::library> coreLibraries;
	public:
		/*
		 Loads all the DLL system cores from the coreSubdirectory (relative to the executable directory)
		 Note: There is no guaranty of load order.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT CoreLoader(std::filesystem::path coreSubdirectory);

		/*
		 Frees all DLL system cores from memory.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT ~CoreLoader();

		/*
		 Get all the currently loaded cores.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT const std::vector<Core*>& getLoadedCores();

		/*
		 Get a currently loaded core with the specified name.
		*/
		LIBGAMEEMU_COMMON_DLL_EXPORT Core* getLoadedCore(std::string name);
	};
}
