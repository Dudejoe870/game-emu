#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	class Logger
	{
	protected:
		virtual void LogInfoImpl(const std::string& info) = 0;
		virtual void LogWarningImpl(const std::string& warning) = 0;
		virtual void LogErrorImpl(const std::string& error) = 0;
	public:
		inline void LogInfo(const std::string& info)
		{
			if (!this) return;
			LogInfoImpl(info);
		}

		inline void LogWarning(const std::string& warning)
		{
			if (!this) return;
			LogWarningImpl(warning);
		}

		inline void LogError(const std::string& error)
		{
			if (!this) return;
			LogErrorImpl(error);
		}
	};
}
