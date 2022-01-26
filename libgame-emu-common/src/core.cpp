#include <game-emu/common/core.h>

#include <dynalo/dynalo.hpp>

namespace GameEmu::Common
{
	Core::Core()
	{

	}

	void Core::InitializeCores(std::filesystem::path coreSubdirectory)
	{
		const std::filesystem::path coresLoadPath = Util::getExecutablePath().parent_path() / coreSubdirectory;
		std::vector<std::string> coreDLLs;
		if (std::filesystem::exists(coresLoadPath))
		{
			for (const auto& entry : std::filesystem::directory_iterator(coresLoadPath))
			{
				if (!entry.is_directory() && entry.path().extension() == "." + dynalo::native::name::extension())
					coreDLLs.push_back(entry.path().string());
			}
		}


	}
}
