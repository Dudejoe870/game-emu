#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/logger.h>

namespace GameEmu::Common
{
	class RunState
	{
	public:
		Logger* logger;

		virtual bool isRunning() = 0;
		virtual bool isPaused() = 0;
	};
}
