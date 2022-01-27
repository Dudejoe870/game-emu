#include <iostream>

#include <game-emu/common/coreloader.h>

using namespace GameEmu;

int main()
{
	Common::CoreLoader coreLoader("cores");

	std::cout << "Currently Loaded Cores:" << std::endl;
	const std::vector<Common::Core*>& loadedCores = coreLoader.getLoadedCores();
	if (loadedCores.empty()) std::cout << "None" << std::endl;
	for (Common::Core* core : loadedCores)
	{
		std::string coreType = "";
		switch (core->getType())
		{
		case Common::Core::Type::Processor:
			coreType = "Processor";
			break;
		case Common::Core::Type::GraphicsRasterizer:
			coreType = "Graphics Rasterizer";
			break;
		case Common::Core::Type::System:
			coreType = "System";
			break;
		}
		std::cout << core->getName() << " (" << coreType << ")" << std::endl;
	}

	return 0;
}
