#include <game-emu/common/core.h>
#include <game-emu/common/coreloader.h>

#include <dynalo/dynalo.hpp>

namespace GameEmu::Common
{
	Core::Core(GameEmu::Common::CoreLoader* loader)
	{
		this->loader = loader;
	}

	std::string Core::getName()
	{
		return "gameemu-core-unknown";
	}

	Core::Type Core::getType()
	{
		return Core::Type::Processor;
	}
}
