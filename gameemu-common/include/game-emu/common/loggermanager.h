#pragma once

#include <game-emu/common/stdcommon.h>
#include <game-emu/common/logger.h>

namespace GameEmu::Common
{
	class LoggerManager
	{
	public:
		virtual std::shared_ptr<Logger> CreateLogger(const std::string& name) = 0;
	};
}
