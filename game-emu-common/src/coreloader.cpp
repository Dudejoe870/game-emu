#include <game-emu/common/coreloader.h>

#include <game-emu/common/core.h>

namespace GameEmu::Common
{
	CoreLoader::CoreLoader(std::filesystem::path coreSubdirectory)
	{
		const std::filesystem::path coresLoadPath = Util::getExecutablePath().parent_path() / coreSubdirectory;

		if (std::filesystem::exists(coresLoadPath))
		{
			for (const auto& entry : std::filesystem::directory_iterator(coresLoadPath))
			{
				if (!entry.is_directory() && entry.path().extension() == "." + dynalo::native::name::extension())
					coreLibraries.push_back(dynalo::library(entry.path().string()));
			}
		}
		else
		{
			std::filesystem::create_directory(coresLoadPath);
			return;
		}

		for (dynalo::library& lib : coreLibraries)
		{
			auto GameEmuInitCore = lib.get_function<Core*(CoreLoader*)>("GameEmuInitCore");
			cores.push_back(GameEmuInitCore(this));
		}

		for (Core* core : cores) core->LoadDependencies();
	}

	CoreLoader::~CoreLoader()
	{
		for (Core* core : cores) delete core;
		cores.clear();
	}

	const std::vector<Core*>& CoreLoader::getLoadedCores()
	{
		return cores;
	}

	Core* CoreLoader::getLoadedCore(std::string name)
	{
		for (Core* core : cores)
		{
			if (core->getName() == name)
				return core;
		}
		return nullptr;
	}
}
