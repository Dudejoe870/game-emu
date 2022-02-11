#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	class Logger
	{
	protected:
		virtual void LogInfoImpl(std::string info) = 0;
		virtual void LogWarningImpl(std::string warning) = 0;
		virtual void LogErrorImpl(std::string error) = 0;
	public:
		inline void LogInfo(std::string info)
		{
			if (!this) return;
			LogInfoImpl(info);
		}

		inline void LogWarning(std::string warning)
		{
			if (!this) return;
			LogWarningImpl(warning);
		}

		inline void LogError(std::string error)
		{
			if (!this) return;
			LogErrorImpl(error);
		}
	};
}
