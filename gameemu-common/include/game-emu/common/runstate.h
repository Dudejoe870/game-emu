#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/loggermanager.h>

namespace GameEmu::Common
{
	class RunState
	{
	protected:
		std::shared_ptr<Logger> logger;
	public:
		LoggerManager& logManager;

		RunState(LoggerManager& logManager)
			: logManager(logManager)
		{
			this->logger = logManager.CreateLogger("game-emu");
		}

		virtual bool IsRunning() = 0;
		virtual bool IsPaused() = 0;
	};
}
