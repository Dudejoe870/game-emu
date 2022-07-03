#include <game-emu/common/coreloader.h>

#include <game-emu/common/core.h>

#include <game-emu/common/util.h>

namespace GameEmu::Common
{
	CoreLoader::CoreLoader(std::filesystem::path coreSubdirectory)
	{
		const std::filesystem::path coresLoadPath = Util::GetExecutablePath().parent_path() / coreSubdirectory;

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
	}

	CoreLoader::~CoreLoader() 
	{
		for (Core* core : cores)
			delete core;
		cores.clear();
	}

	const std::vector<Core*>& CoreLoader::GetLoadedCores()
	{
		return cores;
	}

	Core* CoreLoader::GetLoadedCore(std::string name)
	{
		for (Core* core : cores)
		{
			if (core->GetName() == name)
				return core;
		}
		return nullptr;
	}
}
