#pragma once

#include <game-emu/common/core.h>
#include <game-emu/common/coreloader.h>

namespace GameEmu::Cores::GBZ80
{
	class Core : public GameEmu::Common::Core
	{
	public:
		Core(GameEmu::Common::CoreLoader* loader);

		std::string getName();
		GameEmu::Common::Core::Type getType();
	};
}
