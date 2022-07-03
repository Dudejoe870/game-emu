#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/logger.h>

namespace GameEmu::Common
{
	class RunState
	{
	public:
		Logger* logger;

		RunState()
		{
			this->logger = nullptr;
		}

		RunState(Logger& logger)
		{
			this->logger = &logger;
		}

		virtual bool IsRunning() = 0;
		virtual bool IsPaused() = 0;
	};
}
