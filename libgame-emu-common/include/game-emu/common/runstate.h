#pragma once

namespace GameEmu::Common
{
	class RunState
	{
	public:
		virtual bool isRunning() = 0;
		virtual bool isPaused() = 0;
	};
}
