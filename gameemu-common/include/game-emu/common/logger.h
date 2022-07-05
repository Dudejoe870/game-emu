#pragma once

#include <game-emu/common/stdcommon.h>

namespace GameEmu::Common
{
	/*
	 thread-safe Logger

	 Implementations of this class should be thread-safe
	*/
	class Logger
	{
	protected:
		std::string name;
	public:
		Logger(const std::string& name)
		{
			this->name = name;
		}

		virtual void LogInfo(const std::string& info) = 0;
		virtual void LogWarning(const std::string& warning) = 0;
		virtual void LogError(const std::string& error) = 0;
	};
}
