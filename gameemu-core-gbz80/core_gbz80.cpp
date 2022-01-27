#include "core_gbz80.h"

namespace GameEmu::Cores::GBZ80
{
	Core::Core(GameEmu::Common::CoreLoader* loader)
		: GameEmu::Common::Core(loader)
	{
	}

	std::string Core::getName()
	{
		return "gameemu-core-gbz80";
	}

	GameEmu::Common::Core::Type Core::getType()
	{
		return GameEmu::Common::Core::Type::Processor;
	}
}
